// License: GPL v3 Copyright: 2023, Adwaith Rajesh <adwaithrajesh3180@gmail.com>

module parsers

pub fn get_parser_map() map[string]fn (string) string {
	return {
		'text_plain':           text_plain_parser
		'text_x-java':          text_plain_parser
		'text_x-c':             text_plain_parser
		'text_x-script.python': text_plain_parser
	}
}
