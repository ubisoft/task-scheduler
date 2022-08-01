------------------------------- MODULE MCSPQueue -------------------------------
\*
\* Multi-Consumer-Single-Producer semi-lock-free queue. Semi-lock-free means
\* that the queue has a lock, but it is taken extremely rare.
\*
\* Queue is a list of blocks. Each block is a lock-free bounded queue. Consumers
\* attach to the oldest block, consume it in a lock-free manner, and switch to
\* the next block using a lock.
\*
\* Producer is alone. It writes new data to the newest block, can write data in
\* bulks to reduce number of atomic operations. When the newest block is full,
\* it creates a new one using a lock.
\*
EXTENDS TLC, Integers, Sequences

--------------------------------------------------------------------------------
\*
\* Constant values.
\*

CONSTANT Count
CONSTANT BlockSize
CONSTANT NULL
CONSTANT GARBAGE
CONSTANT ConsumerIDs

BlockSizeEx == BlockSize + 1

\* Consumers are symmetrical. Meaning that changing their places in different
\* runs won't change the result.
Perms == Permutations(ConsumerIDs)

--------------------------------------------------------------------------------
\*
\* Variables.
\*

VARIABLE ProducerState
\* Number of items waiting to be flushed to become visible to the consumers.
VARIABLE PendingCount
VARIABLE RecvCount
VARIABLE SentCount
\* An array of blocks.
VARIABLE Queue
\* Not the same as the built-in length. It is updated separately like in real
\* code.
VARIABLE QueueSize
\* Mutex.
VARIABLE LockCount
\* An array of consumers.
VARIABLE Consumers

ConsumerVars == <<Consumers, RecvCount>>

ProducerVars == <<ProducerState, PendingCount, SentCount>>

vars == <<Queue, QueueSize, LockCount, ConsumerVars, ProducerVars>>

--------------------------------------------------------------------------------
\*
\* Helper functions.
\*

\* In TLA+ there is no convenient way to change multiple fields of a struct,
\* especially if it is in an array of other structs. Functions here help to make
\* it look sane. For that the order of arguments is inverted - the object to
\* change goes last. Thanks to that, multiple mutations can be written as
\*
\*   SetField(key1, val1,
\*   SetField(key2, val2,
\*   SetField(key3, val3,
\*   Object)))
\*
\* Almost like normal assignment. This is additionally simplified by the fact
\* that there are no struct types. The same setter can be used for any struct,
\* only member name matters.

SetData(i, v, b) == [b EXCEPT !.data[i] = v]
SetFlushIdx(v, b) == [b EXCEPT !.flush_idx = v]
SetWriteIdx(v, b) == [b EXCEPT !.write_idx = v]
SetReadIdx(v, b) == [b EXCEPT !.read_idx = v]
SetBlockIdx(v, b) == [b EXCEPT !.block_idx = v]
SetPending(v, b) == [b EXCEPT !.pending = v]
SetState(v, b) == [b EXCEPT !.state = v]

\* The same but for struct arrays.

ArrLen(s) == Len(s)
ArrLast(s) == s[Len(s)]
ArrSet(i, v, s) == [s EXCEPT ![i] = v]
ArrSetLast(v, s) == ArrSet(ArrLen(s), v, s)
ArrAppend(v, s) == Append(s, v)
ArrSetLastData(i, v, s) == ArrSetLast(SetData(i, v, ArrLast(s)), s)
ArrSetLastFlushIdx(v, s) == ArrSetLast(SetFlushIdx(v, ArrLast(s)), s)
ArrSetLastWriteIdx(v, s) == ArrSetLast(SetWriteIdx(v, ArrLast(s)), s)
ArrSetReadIdx(i, v, s) == ArrSet(i, SetReadIdx(v, s[i]), s)
ArrSetBlockIdx(i, v, s) == ArrSet(i, SetBlockIdx(v, s[i]), s)
ArrSetLastPending(v, s) == ArrSetLast(SetPending(v, ArrLast(s)), s)
ArrSetState(i, v, s) == ArrSet(i, SetState(v, s[i]), s)

\* Constructors.

BlockNew == [
  write_idx |-> 1,
  read_idx |-> 1,
  flush_idx |-> 1,
  pending |-> NULL,
  \* First element is initialized. The rest should be set on fly.
  data |-> ArrSet(1, NULL, [i \in 1..BlockSizeEx |-> GARBAGE])
]

ConsumerNew == [
  state |-> "idle",
  block_idx |-> 1,
  read_idx |-> NULL
]

Init ==
  /\ ProducerState = "idle"
  /\ PendingCount = 0
  /\ RecvCount = 0
  /\ SentCount = 0
  /\ Queue = <<BlockNew>>
  /\ QueueSize = 0
  /\ LockCount = 0
  /\ Consumers = [c \in ConsumerIDs |-> ConsumerNew]

LastBlock == ArrLast(Queue)
LastBlockHasPending == LastBlock.pending # NULL
LastBlockHasSpace == LastBlock.write_idx # BlockSizeEx

LockTake ==
  /\ LockCount = 0
  /\ LockCount' = LockCount + 1

LockPut ==
  /\ LockCount > 0
  /\ LockCount' = LockCount - 1

CanPushMore ==
  /\ ProducerState = "idle"
  /\ SentCount < Count

\* In the steps below the splitting is done by actions which are visible to
\* other participants. For example, push changes counter and some queue
\* elements. All are visible to consumers. Hence must be done in separate steps
\* to give the consumers a chance to notice these changes and handle them
\* correctly.
\* Actions, which do not change globally visible data or are atomic, can be done
\* in one step. For example, an atomic increment can be done as a single step.
\* Without spitting read and write.

--------------------------------------------------------------------------------
\* Producer adds new block when filled the latest one.

PushNextBlockStart ==
  /\ CanPushMore
  /\ ~LastBlockHasSpace
  /\ ~LastBlockHasPending
  \* ---
  /\ LockTake
  /\ ProducerState' = "push_next_block_locked"
  /\ UNCHANGED<<PendingCount, SentCount, Queue, QueueSize, ConsumerVars>>

PushNextBlockLocked ==
  /\ ProducerState = "push_next_block_locked"
  \* ---
  /\ Queue' = ArrAppend(BlockNew, Queue)
  /\ ProducerState' = "push_next_block_unlock"
  /\ UNCHANGED<<PendingCount, SentCount, QueueSize, LockCount, ConsumerVars>>

PushNextBlockUnlock ==
  /\ ProducerState = "push_next_block_unlock"
  \* ---
  /\ LockPut
  /\ ProducerState' = "idle"
  /\ UNCHANGED<<PendingCount, SentCount, Queue, QueueSize, ConsumerVars>>

PushNextBlock ==
  \/ PushNextBlockStart
  \/ PushNextBlockLocked
  \/ PushNextBlockUnlock

--------------------------------------------------------------------------------
\* Producer can flush pending data if there is any.

PushFlushBlockStart ==
  /\ ProducerState = "idle"
  /\ LastBlockHasPending
  \* ---
  /\ QueueSize' = QueueSize + PendingCount
  /\ PendingCount' = 0
  /\ ProducerState' = "flush_block_do"
  /\ UNCHANGED<<LockCount, SentCount, Queue, ConsumerVars>>

PushFlushBlockDo ==
  /\ ProducerState = "flush_block_do"
  \* ---
  /\ Queue' = ArrSetLastData(LastBlock.write_idx, NULL, Queue)
  /\ ProducerState' = "flush_block_end"
  /\ UNCHANGED<<LockCount, PendingCount, SentCount, QueueSize, ConsumerVars>>

PushFlushBlockEnd ==
  /\ ProducerState = "flush_block_end"
  \* ---
  /\ Queue' = ArrSetLastData(LastBlock.flush_idx, LastBlock.pending,
              ArrSetLastFlushIdx(LastBlock.write_idx,
              ArrSetLastPending(NULL,
              Queue)))
  /\ ProducerState' = "idle"
  /\ UNCHANGED<<LockCount, PendingCount, SentCount, QueueSize, ConsumerVars>>

PushFlushBlock ==
  \/ PushFlushBlockStart
  \/ PushFlushBlockDo
  \/ PushFlushBlockEnd

--------------------------------------------------------------------------------
\* Producer normal push.

PushStart ==
  /\ CanPushMore
  /\ LastBlockHasSpace
  /\ ~LastBlockHasPending
  \* ---
  /\ QueueSize' = QueueSize + 1
  /\ ProducerState' = "push_prepare"
  /\ UNCHANGED<<LockCount, PendingCount, SentCount, Queue, ConsumerVars>>

PushPrepare ==
  /\ ProducerState = "push_prepare"
  \* ---
  /\ Queue' = ArrSetLastData(LastBlock.write_idx + 1, NULL, Queue)
  /\ ProducerState' = "push_end"
  /\ UNCHANGED<<LockCount, PendingCount, SentCount, QueueSize, ConsumerVars>>

PushEnd ==
  /\ ProducerState = "push_end"
  \* ---
  /\ SentCount' = SentCount + 1
  /\ Queue' = ArrSetLastData(LastBlock.flush_idx, SentCount',
              ArrSetLastWriteIdx(LastBlock.write_idx + 1,
              ArrSetLastFlushIdx(LastBlock.write_idx + 1,
              Queue)))
  /\ ProducerState' = "idle"
  /\ UNCHANGED<<LockCount, PendingCount, QueueSize, ConsumerVars>>

Push ==
  \/ PushStart
  \/ PushPrepare
  \/ PushEnd

--------------------------------------------------------------------------------
\* Producer can push multiple pending items and flush them all together. This is
\* much faster than pushing one-by-one synchronously, because uses 0 atomic
\* operations, and the flushing in the end costs the same as a normal push.
\*
\* The trick is to keep one NULL between the consumers and the pending data. The
\* consumers won't go beyond NULL, thus the producer can touch data after this
\* NULL anyhow, it is safe.

PushPending ==
  /\ CanPushMore
  /\ LastBlockHasSpace
  \* ---
  /\ SentCount' = SentCount + 1
  /\ PendingCount' = PendingCount + 1
  /\ IF LastBlockHasPending THEN
     /\ Queue' = ArrSetLastData(LastBlock.write_idx, SentCount',
                 ArrSetLastWriteIdx(LastBlock.write_idx + 1,
                 Queue))
     ELSE
     /\ Queue' = ArrSetLastPending(SentCount',
                 ArrSetLastWriteIdx(LastBlock.write_idx + 1,
                 Queue))
  /\ UNCHANGED<<ProducerState, LockCount, QueueSize, ConsumerVars>>

--------------------------------------------------------------------------------
\* Consumer pop.

PopStart(i) ==
  LET c == Consumers[i]
      block_read_idx == Queue[c.block_idx].read_idx IN
  /\ c.state = "idle"
  /\ block_read_idx < BlockSizeEx
  \* ---
  /\ Consumers' = ArrSetState(i, "pop_check_value",
                  ArrSetReadIdx(i, block_read_idx,
                  Consumers))
  /\ UNCHANGED<<Queue, QueueSize, LockCount, RecvCount, ProducerVars>>

PopCheckValue(i) ==
  LET c == Consumers[i]
      data == Queue[c.block_idx].data IN
  /\ c.state = "pop_check_value"
  \* ---
  /\ IF data[c.read_idx] = NULL THEN
     /\ Consumers' = ArrSetState(i, "idle",
                     ArrSetReadIdx(i, NULL,
                     Consumers))
     ELSE
     /\ Consumers' = ArrSetState(i, "pop_check_idx", Consumers)
  /\ UNCHANGED<<Queue, QueueSize, LockCount, RecvCount, ProducerVars>>

PopCheckIdx(i) ==
  LET c == Consumers[i]
      block == Queue[c.block_idx]
      data == block.data
      block_read_idx == block.read_idx IN
  /\ c.state = "pop_check_idx"
  \* ---
  /\ IF block_read_idx = c.read_idx THEN
     /\ RecvCount' = RecvCount + 1
     /\ Assert(RecvCount' = data[c.read_idx], "Received value")
     /\ QueueSize' = QueueSize - 1
     /\ Queue' = ArrSetReadIdx(c.block_idx, c.read_idx + 1, Queue)
     ELSE
     /\ UNCHANGED<<RecvCount, QueueSize, Queue>>
  /\ Consumers' = ArrSetState(i, "idle",
                  ArrSetReadIdx(i, NULL,
                  Consumers))
  /\ UNCHANGED<<LockCount, ProducerVars>>

Pop(i) ==
  \/ PopStart(i)
  \/ PopCheckValue(i)
  \/ PopCheckIdx(i)

--------------------------------------------------------------------------------
\* Consumer switch to next block.

PopNextBlockStart(i) ==
  LET c == Consumers[i]
      block_read_idx == Queue[c.block_idx].read_idx IN
  /\ c.state = "idle"
  /\ block_read_idx = BlockSizeEx
  \* ---
  /\ LockTake
  /\ Consumers' = ArrSetState(i, "next_block_locked", Consumers)
  /\ UNCHANGED<<Queue, QueueSize, RecvCount, ProducerVars>>

PopNextBlockLocked(i) ==
  LET c == Consumers[i] IN
  /\ c.state = "next_block_locked"
  \* ---
  /\ IF c.block_idx < ArrLen(Queue) THEN
     /\ Consumers' = ArrSetState(i, "next_block_unlock",
                     ArrSetBlockIdx(i, c.block_idx + 1,
                     Consumers))
     ELSE
     /\ Consumers' = ArrSetState(i, "next_block_unlock", Consumers)
  /\ UNCHANGED<<Queue, QueueSize, LockCount, RecvCount, ProducerVars>>

PopNextBlockUnlock(i) ==
  LET c == Consumers[i] IN
  /\ c.state = "next_block_unlock"
  \* ---
  /\ LockPut
  /\ Consumers' = ArrSetState(i, "idle", Consumers)
  /\ UNCHANGED<<Queue, QueueSize, RecvCount, ProducerVars>>

PopNextBlock(i) ==
  \/ PopNextBlockStart(i)
  \/ PopNextBlockLocked(i)
  \/ PopNextBlockUnlock(i)

--------------------------------------------------------------------------------
\*
\* Actions.
\*

PushAny ==
  \/ PushNextBlock
  \/ PushFlushBlock
  \/ Push
  \/ PushPending

PopAny(i) ==
  \/ Pop(i)
  \/ PopNextBlock(i)

Next ==
  \/ PushAny
  \/ \E i \in ConsumerIDs: PopAny(i)

--------------------------------------------------------------------------------
\*
\* Invariants.
\*

\* In the end all data must be sent, all delivered, and the queue is empty.
TerminalProperty == <>[](
  /\ SentCount = Count
  /\ RecvCount = Count
  /\ QueueSize = 0)

QueueInvariant ==
  /\ \A i \in 1..ArrLen(Queue): LET b == Queue[i] IN
     /\ b.write_idx <= BlockSizeEx
     /\ b.read_idx <= BlockSizeEx
     /\ b.flush_idx <= BlockSizeEx
  /\ QueueSize >= 0

ConsumerInvariant ==
  \A i \in ConsumerIDs: LET c == Consumers[i] IN
  /\ c.block_idx <= ArrLen(Queue)
  /\ \/ c.read_idx = NULL
     \/ /\ c.read_idx <= BlockSize
        \* Garbage is never visible to consumers.
        /\ Queue[c.block_idx].data[c.read_idx] # GARBAGE

LockInvariant == LockCount = 0 \/ LockCount = 1

TotalInvariant ==
  /\ QueueInvariant
  /\ ConsumerInvariant
  /\ LockInvariant

Spec ==
  /\ Init
  /\ [][Next]_vars
  /\ SF_vars(PushAny)
  /\ \A i \in ConsumerIDs: WF_vars(PopAny(i))

================================================================================
