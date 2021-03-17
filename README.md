# MySQL Test Run (MTR) checker utility
This utility is suposed to check for orphaned files in the MTR file structure

## Building
```
git clone https://github.com/Percona-Lab/mtr-checker.git
cd mtr-checker
cmake
make
```

## Usage
```
mtr_checker <log_level> <test_dir> [<result_dir>]
```
**<log_level>** can be one of the following
* *error* - print info about found inconsistencies (e.g. .result file without corresponding .test file, etc.)
* *warning* - also include info about files with unknown extensions found in the directories being processed (like .inc, .sql, etc.)
* *info* - also include info about succesfully verified test cases.

**<test_dir>** - the directory where .test files are located.

**<result_dir>** - the directory where .result files are located.

For checking test suites where .test and .result files are located in different directories, run the utility with 3 arguments.
For instance,
```
mtr_checker error mysql-test/t mysql-test/r
```

For processing test suites where both .test and .result files are located in the same directory, run the utility with 2 arguments.
For instance,
```
mtr_checker error plugin/keyring_vault/tests/mtr
```

## Output samples
```
mtr_checker error mysql-test/suite/rpl/t mysql-test/suite/rpl/r 
```
```
[error]: found unknown file "rpl_critical_errors.result.txt"
[error]: inconsistent test case "rpl_auto_increment": [ opt_master ]
[error]: inconsistent test case "rpl_row_img": [ cnf ]
[error]: inconsistent test case "rpl_innodb": [ opt_master ]
[error]: inconsistent test case "rpl_row_err_daisychain": [ opt_master, opt_slave ]
[error]: inconsistent test case "rpl_mixed_row_innodb": [ opt_master ]
[error]: inconsistent test case "rpl_partial_revokes": [ opt_master, opt_slave ]
[error]: inconsistent test case "rpl_replication_observers_example_plugin_abort_service": [ opt_master ]
[error]: inconsistent test case "rpl_row_jsondiff": [ cnf ]
[error]: inconsistent test case "rpl_row_jsondiff_basic": [ cnf ]
[error]: inconsistent test case "rpl_server_uuid": [ opt_master ]
```
