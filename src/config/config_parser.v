// License: GPL v3 Copyright: 2023, Adwaith Rajesh <adwaithrajesh3180@gmail.com>
module config

import os
import json

pub struct InitConfig {
pub:
	index_dir string
}

pub struct Init {
pub:
	config InitConfig
	ignore []string
}

struct Query {
}

pub struct Config {
pub:
	init  Init
	query Query
}

pub fn parse_config_file(filepath string) Config {
	file_contents := os.read_file(filepath) or {
		eprintln('could not open file: ${filepath}: ${err}')
		exit(1)
	}.normalize_tabs(4)

	config := json.decode(Config, file_contents) or {
		eprintln('Could not parse ${filepath} : ${err}')
		exit(1)
	}

	return config
}
