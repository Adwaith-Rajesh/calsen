// License: GPL v3 Copyright: 2023, Adwaith Rajesh <adwaithrajesh3180@gmail.com>

module parsers

#flag -lmagic
#include "magic.h"

fn C.magic_open(int) C.magic_t
fn C.magic_load(C.magic_t, &char)
fn C.magic_file(C.magic_t, &char) &char
fn C.magic_close(C.magic_t)

pub fn get_parser_map() map[string]fn (string) string {
	return {
		'text_plain':           text_plain_parser
		'text_x-java':          text_plain_parser
		'text_x-c':             text_plain_parser
		'text_x-script.python': text_plain_parser
	}
}

pub fn get_file_parser(filepath string) ?fn (string) string {
	m := C.magic_open(C.MAGIC_MIME_TYPE)
	defer {
		C.magic_close(m)
	}
	C.magic_load(m, 0)
	mime_type := (unsafe { (C.magic_file(m, filepath.str)).vstring() }).replace('/', '_')
	parser_map := get_parser_map()

	return parser_map[mime_type] or { return none }
}
