import argparse
import json
import os
import shlex
import subprocess
import sys

# Big integer to a string with thousands separated by spaces. For example,
# '1 000 000' instead of '1000000'.
def bigintstr(n):
	return '{:,}'.format(n).replace(',', ' ')

# Locate the exe file in a given dir. It helps not to specify full paths to exes in the
# config file and make it re-usable on different platforms.
def find_exe_file(name):
	if os.path.exists(name):
		return name
	name_win = name + '.exe'
	for root, subdirs, files in os.walk(glob_exedir):
		if name in files:
			return os.path.join(root, name)
		if name_win in files:
			return os.path.join(root, name_win)
	print('Not found exe {} in {}'.format(name, glob_exedir))
	sys.exit(-1)

# Description of one version to run.
class Version:
	def __init__(self, cfg):
		# Full name of the version. Like 'Canon Task Scheduler'.
		self.name = cfg['name']
		# Short name to use in sentences, like 'canon scheduler'.
		self.short_name = cfg['short_name']
		# File name or full path to the bench exe having this version implemented.
		self.exe = find_exe_file(cfg['exe'])
		# Additional command line arguments. It helps to test same version with different
		# args. For example, a multi-consumer queue with subqueue sizes 1000, 5000, 10000,
		# etc.
		self.cmd = cfg.get('cmd')
		if not self.cmd:
			self.cmd = ''
		print('==== Version {}'.format(self.name))
		print('== Found exe {}'.format(self.exe))

# Results of running a given exe one time.
class SingleRun:
	def __init__(self, args, exe):
		proc = subprocess.Popen(args, executable=exe, stdout=subprocess.PIPE)
		report, err = proc.communicate()
		if err:
			print('Failed to run: {}'.format(err))
			sys.exit(-1)
		lines = []
		self.value = None
		for i, l in enumerate(report.decode('utf-8').splitlines()):
			# Remove comments.
			if l.startswith('='):
				continue
			lines.append(l)
			# Find the target metric. Include ':' to filter out metrics having the target
			# one as a prefix.
			if l.find(glob_metric_key + ':') < 0:
				continue
			if self.value:
				print('Found the target metric more than once')
				sys.exit(-1)
			self.value = int(l.split(':')[-1].strip())
		if not self.value:
			print('Not found the target metric')
			sys.exit(-1)
		# Rebuild the report without the comments.
		self.report = '\n'.join(lines)

	# Sorting makes sense when want to build aggregated statistics like
	# min/median/max values.
	def __lt__(self, other):
		return self.value < other.value

# Aggregated results of multiple runs of one version.
class Report:
	def __init__(self, runs):
		runs = sorted(runs)
		median = runs[len(runs) // 2]
		self.value_min = runs[0].value
		self.value_median = median.value
		self.value_max = runs[-1].value
		self.report_median = median.report

# Description of a scenario to run against multiple versions.
class Scenario:
	def __init__(self, cfg):
		# Full name of the scenario to use as a header.
		self.name = cfg['name']
		# Common command line args to give to each version.
		self.cmd = cfg['cmd']
		# How many times to run. The bigger the count, the less results are affected
		# by noise.
		self.count = cfg['count']
		if self.count < 3:
			print('Run count {} is too small'.format(self.count))
			sys.exit(-1)

	def run(self):
		print('==== Scenario: {}'.format(self.name))
		reports = {}
		for version_key, version in glob_versions.items():
			runs = []
			print('== Version: {}'.format(version.name))
			exe = version.exe
			args = shlex.split('{} {}'.format(self.cmd, version.cmd))
			args.insert(0, exe)
			for i in range(self.count):
				r = SingleRun(args, exe)
				runs.append(r)
				print('= [{}]: {}: {}'.format(i + 1, glob_metric_key, r.value))
			print()
			reports[version_key] = Report(runs)
		return reports

#
# Write a report about a single version.
#
def write_version(out_file, version_key, report):
	version = glob_versions[version_key]
	out_file.write('#### {}\n'.format(version.name))
	out_file.write('== Aggregated report:\n')
	out_file.write('{} min:    {:>12}\n'.format(
		glob_metric_key, bigintstr(report.value_min)))
	out_file.write('{} median: {:>12}\n'.format(
		glob_metric_key, bigintstr(report.value_median)))
	out_file.write('{} max:    {:>12}\n'.format(
		glob_metric_key, bigintstr(report.value_max)))
	out_file.write('\n')

	out_file.write('== Median report:\n')
	out_file.write(report.report_median)

#
# Write a report about an entire scenario with all its versions.
#
def write_scenario(out_file, scenario):
	out_file.write('\n---\n')
	out_file.write('**{}**\n'.format(scenario.name))
	out_file.write('\n```\n');
	out_file.write('Command line args: {}\n'.format(scenario.cmd))
	out_file.write('Run count: {}\n'.format(scenario.count))
	out_file.write('Summary:\n')

	reports = scenario.run()
	main_report = reports[glob_main_version_key]
	# The main metric's value.
	out_file.write('* {} {};\n'.format(
		bigintstr(main_report.value_median), glob_metric_name))

	# Compare the main version against other versions by the main metric value.
	for version_key, rs in reports.items():
		# Don't compare against self.
		if version_key == glob_main_version_key:
			continue
		ratio = main_report.value_median / rs.value_median
		short_name = glob_versions[version_key].short_name
		if abs(ratio - 1) <= glob_precision:
			out_file.write('* almost same as {};\n'.format(short_name))
		else:
			out_file.write('* x{:.3f} of {};\n'.format(ratio, short_name))
	out_file.write('```\n')

	out_file.write('\n<details><summary>Details</summary>\n')
	out_file.write('\n```\n');
	# Write main version first.
	write_version(out_file, glob_main_version_key, reports[glob_main_version_key])
	# And then the others.
	for version_key, report in reports.items():
		if version_key == glob_main_version_key:
			continue
		out_file.write('\n\n')
		write_version(out_file, version_key, report)
	out_file.write('```\n</details>\n')

#
# Write a full report about all scenarios.
#
def write_report(out_file, scenarios):
	# Add an explicit <br>. Otherwise somewhy GitHub tries to show these 2 lines as one.
	out_file.write('**OS**: {}<br>\n'.format(config['os']))
	out_file.write('**CPU**: {}\n'.format(config['cpu']))

	for s in scenarios:
		write_scenario(out_file, s)

parser = argparse.ArgumentParser(description='Build benchmark report in markdown format')
parser.add_argument('--config', type=str, help='path to configuration file',
	required=True)
parser.add_argument('--out', type=str, help='path to output file',
	required=True)
parser.add_argument('--exedir', type=str, help='path to directory with exe files',
	default='.')
args = parser.parse_args()

with open(args.config, 'r') as config_file:
	config = json.load(config_file)

glob_main_version_key = config['main_version']
glob_metric_key = config['metric_key']
glob_metric_name = config['metric_name']
glob_precision = config['precision']
glob_exedir = args.exedir

glob_versions = {}
for k, v in config['versions'].items():
	glob_versions[k] = Version(v)

scenarios = []
for v in config['scenarios']:
	scenarios.append(Scenario(v))

out_file = open(args.out, 'w')
write_report(out_file, scenarios)
out_file.close()
