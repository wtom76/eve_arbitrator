-- DROP SCHEMA public;

CREATE SCHEMA public AUTHORIZATION postgres;

-- DROP TYPE public.agent;

CREATE TYPE public.agent AS (
	"name" text,
	"system" text,
	region text,
	account text);

-- DROP TYPE public.route;

CREATE TYPE public.route AS (
	from_system_id int8,
	to_system_id int8,
	distance int8);

-- DROP TYPE public."system";

CREATE TYPE public."system" AS (
	id int8,
	"name" text,
	security_status numeric,
	constellation_id int8);

-- DROP TYPE public."type";

CREATE TYPE public."type" AS (
	id int8,
	group_id int8,
	market_group_id int8,
	packaged_volume numeric,
	"name" text);

-- DROP TYPE public."_agent";

CREATE TYPE public."_agent" (
	INPUT = array_in,
	OUTPUT = array_out,
	RECEIVE = array_recv,
	SEND = array_send,
	ANALYZE = array_typanalyze,
	ALIGNMENT = 8,
	STORAGE = any,
	CATEGORY = A,
	ELEMENT = public.agent,
	DELIMITER = ',');

-- DROP TYPE public."_route";

CREATE TYPE public."_route" (
	INPUT = array_in,
	OUTPUT = array_out,
	RECEIVE = array_recv,
	SEND = array_send,
	ANALYZE = array_typanalyze,
	ALIGNMENT = 8,
	STORAGE = any,
	CATEGORY = A,
	ELEMENT = public.route,
	DELIMITER = ',');

-- DROP TYPE public."_system";

CREATE TYPE public."_system" (
	INPUT = array_in,
	OUTPUT = array_out,
	RECEIVE = array_recv,
	SEND = array_send,
	ANALYZE = array_typanalyze,
	ALIGNMENT = 8,
	STORAGE = any,
	CATEGORY = A,
	ELEMENT = public."system",
	DELIMITER = ',');

-- DROP TYPE public."_type";

CREATE TYPE public."_type" (
	INPUT = array_in,
	OUTPUT = array_out,
	RECEIVE = array_recv,
	SEND = array_send,
	ANALYZE = array_typanalyze,
	ALIGNMENT = 8,
	STORAGE = any,
	CATEGORY = A,
	ELEMENT = public."type",
	DELIMITER = ',');
-- public.agent definition

-- Drop table

-- DROP TABLE public.agent;

CREATE TABLE public.agent (
	"name" text NOT NULL,
	"system" text NULL,
	region text NULL,
	account text NULL,
	CONSTRAINT agent_pk PRIMARY KEY (name)
);


-- public.route definition

-- Drop table

-- DROP TABLE public.route;

CREATE TABLE public.route (
	from_system_id int8 NOT NULL,
	to_system_id int8 NOT NULL,
	distance int8 NULL,
	CONSTRAINT route_un UNIQUE (from_system_id, to_system_id)
);


-- public."system" definition

-- Drop table

-- DROP TABLE public."system";

CREATE TABLE public."system" (
	id int8 NOT NULL,
	"name" text NULL,
	security_status numeric NULL,
	constellation_id int8 NULL,
	CONSTRAINT system_pk PRIMARY KEY (id)
);
CREATE UNIQUE INDEX system_name_idx ON public.system USING btree (name);


-- public."type" definition

-- Drop table

-- DROP TABLE public."type";

CREATE TABLE public."type" (
	id int8 NOT NULL,
	group_id int8 NULL,
	market_group_id int8 NULL,
	packaged_volume numeric NULL,
	"name" text NULL,
	CONSTRAINT type_pk PRIMARY KEY (id)
);
