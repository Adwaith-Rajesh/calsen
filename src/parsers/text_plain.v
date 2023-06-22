// License: GPL v3 Copyright: 2023, Adwaith Rajesh <adwaithrajesh3180@gmail.com>

module parsers

import os

fn text_plain_parser(filepath string) string {
	file_contents := os.read_file(filepath) or {
		eprintln('could not read ${filepath} : ${err}')
		return ''
	}
	return file_contents
}
