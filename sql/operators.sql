CREATE TABLE shifts (
"sh_id" int4 NOT NULL,
"sh_name" varchar(100),
"sh_note" varchar(100),
"sh_location" int4 NOT NULL,
PRIMARY KEY ("sh_id", "sh_location")
);

CREATE TABLE operator_jobs (
oj_id int,
oj_name varchar(100),
oj_location int NOT NULL,
PRIMARY KEY (oj_id)
);

CREATE TABLE operators (
op_date_begin_education date,
op_date_attestation date,
op_remove_attestation date,
op_temp bool NOT NULL DEFAULT false,
op_attestator int NOT NULL,
op_job int NOT NULL,
op_shift int NOT NULL,
PRIMARy KEY (co_id)
) INHERITS(contractors);