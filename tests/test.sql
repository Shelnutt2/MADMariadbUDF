create table test.test_int_mad (value int);
insert into test.test_int_mad (value) VALUES (1),(1),(2),(2),(4),(6),(9);

create table test.test_double_mad (value double);
insert into test.test_double_mad (value) VALUES (1),(1),(2),(2),(4),(6),(9);

create table test.test_decimal_mad (value decimal);
insert into test.test_decimal_mad (value) VALUES (1),(1),(2),(2),(4),(6),(9);

create table test.test_int_outlier (value int);
insert into test.test_int_outlier (value) VALUES (1),(2),(3),(4),(5),(6),(7),(8),(9);

create table test.test_double_outlier (value double);
insert into test.test_double_outlier (value) VALUES (53),(55),(51),(50),(60),(52),(100),(0);
