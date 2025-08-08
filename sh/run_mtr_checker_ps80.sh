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
binlog_57_decryption
binlog_gtid
binlog_nogtid
clone
collations
component_encryption_udf
component_keyring_file
component_keyring_kms
component_masking_functions
component_percona_telemetry
connection_control
data_masking
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
junit
junit_combinations
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
rocksdb
rocksdb_clone
rocksdb_rpl
rocksdb_stress
rocksdb_sys_vars
rpl
rpl_encryption
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
tokudb
tokudb_add_index
tokudb_alter_table
tokudb_backup
tokudb_bugs
tokudb_deprecation
tokudb_parts
tokudb_perfschema
tokudb_rpl
tokudb_sys_vars
x
)

ENGINES_SUITES=(
funcs
iuds
rr_trx
)

KEYRING_PLAIN_SUITES=(
component_keyring_kmip
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

for suite in "${KEYRING_PLAIN_SUITES[@]}"
do
  echo "***"
  echo "*** Suite: $suite ***"
  echo "***"
  ${CHECKER_EXE} ${LOG_LEVEL} ${SOURCE_DIR}/mysql-test/suite/$suite/t
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
