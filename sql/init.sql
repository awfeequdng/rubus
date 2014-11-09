CREATE TABLE locations (
lo_id	serial,
lo_name	varchar(100) NOT NULL,
PRIMARY KEY (lo_id)
);

CREATE TABLE contractors(
co_id	serial	NOT NULL,
co_name	varchar(100)	NOT NULL,
co_type	int2	NOT NULL DEFAULT 1,
PRIMARY KEY (co_id)
);

CREATE TABLE user_params (
up_role name,
up_name	varchar(100)	NOT NULL,
up_contractor	int,
up_params json,
up_gui xml,
PRIMARY KEY (up_role)
);

CREATE TABLE units (
un_id	varchar(4)	NOT NULL,
un_name	varchar(20)	NOT NULL,
un_desc varchar(100),
PRIMARY KEY (un_id)
);

CREATE TABLE items (
it_id	serial	,
it_name	varchar(100)	NOT NULL,
it_desc	varchar(300)	,
it_article	varchar(80)	,
it_unit	varchar(4)	NOT NULL,
it_type	varchar(4) NOT NULL,
it_active bool NOT NULL DEFAULT '1',
PRIMARY KEY(it_id),
FOREIGN KEY (it_unit ) REFERENCES units ( un_id )  ON DELETE NO ACTION ON UPDATE CASCADE
);

CREATE TABLE item_images (
ii_item	int,
ii_thumbnail	bytea,
ii_original	bytea,
PRIMARY KEY (ii_item)		,
FOREIGN KEY (ii_item) REFERENCES items ( it_id )  ON DELETE CASCADE ON UPDATE CASCADE
);

CREATE TABLE reports (
re_id serial,
re_name varchar(100) NOT NULL,
re_type int NOT NULL,
re_menu varchar(100) NOT NULL,
re_note text,
re_data xml,
PRIMARY KEY (re_id)
);
