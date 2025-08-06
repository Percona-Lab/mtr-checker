#!/bin/bash

# Check if exactly 3 arguments are provided
if [ "$#" -ne 3 ]; then
  echo "Usage: $0 <checker_exe> <source_dir> <log_level>"
  exit 1
fi

CHECKER_EXE=$1
SOURCE_DIR=$2
LOG_LEVEL=$3

SUITES=(
audit_null
auth_sec
binlog
binlog_gtid
binlog_nogtid
clone
collations
component_keyring_file
connection_control
encryption
federated
funcs_1
funcs_2
gcol
gcol_ndb
gis
group_replication
information_schema
innodb
innodb_fts
innodb_gis
innodb_stress
innodb_undo
innodb_zip
interactive_utilities
jp
json
json_ndb
large_tests
lock_order
max_parts
memcached
ndb
ndb_binlog
ndb_rpl
network_namespace
opt_trace
parts
perfschema
query_rewrite_plugins
rpl
rpl_gtid
rpl_ndb
rpl_nogtid
secondary_engine
service_status_var_registration
service_sys_var_registration
service_udf_registration
special
stress
sys_vars
sysschema
test_service_sql_api
test_services
x
)

ENGINES_SUITES=(
funcs
iuds
rr_trx
)

NDB_PLAIN_SUITES=(
ndb_big
ndb_ddl
ndb_opt
ndbcluster
ndbcrunch
)

PLUGIN_SUITES=(
audit_log_filter
)

PLUGIN_PLAIN_SUITES=(
audit_log
auth_ldap
keyring_vault
percona-pam-for-mysql
procfs
)

echo "***"
echo "*** Suite: main ***"
echo "***"
${CHECKER_EXE} ${LOG_LEVEL} ${SOURCE_DIR}/mysql-test/t ${SOURCE_DIR}/mysql-test/r

for suite in "${SUITES[@]}"
do
  echo "***"
  echo "*** Suite: $suite ***"
  echo "***"
  ${CHECKER_EXE} ${LOG_LEVEL} ${SOURCE_DIR}/mysql-test/suite/$suite/t ${SOURCE_DIR}/mysql-test/suite/$suite/r
  echo
done

for suite in "${ENGINES_SUITES[@]}"
do
  echo "***"
  echo "*** Suite: engines/$suite ***"
  echo "***"
  ${CHECKER_EXE} ${LOG_LEVEL} ${SOURCE_DIR}/mysql-test/suite/engines/$suite/t ${SOURCE_DIR}/mysql-test/suite/engines/$suite/r
  echo
done

for suite in "${NDB_PLAIN_SUITES[@]}"
do
  echo "***"
  echo "*** Suite: $suite ***"
  echo "***"
  ${CHECKER_EXE} ${LOG_LEVEL} ${SOURCE_DIR}/mysql-test/suite/$suite
  echo
done

for suite in "${PLUGIN_SUITES[@]}"
do
  echo "***"
  echo "*** Plugin suite: $suite ***"
  echo "***"
  ${CHECKER_EXE} ${LOG_LEVEL} ${SOURCE_DIR}/plugin/$suite/tests/mtr/t ${SOURCE_DIR}/mysql-test/../plugin/$suite/tests/mtr/r
  echo
done

for suite in "${PLUGIN_PLAIN_SUITES[@]}"
do
  echo "***"
  echo "*** Plugin suite: $suite ***"
  echo "***"
  ${CHECKER_EXE} ${LOG_LEVEL} ${SOURCE_DIR}/plugin/$suite/tests/mtr
  echo
done
