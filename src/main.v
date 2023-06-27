/*
Calsen. A search engine for files
Copyright (C) 2023 Adwaith Rajesh

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

module main

import cli { Command, Flag }
import config { parse_config_file }
import indexer { get_files_to_index }
import lexer
import tf_idf
import parsers { get_file_parser }
import os

fn call_calsen_search(cmd Command) ! {
	indexes := cmd.flags.get_strings('index')!
	query := cmd.flags.get_string('query')!
	search_files(query, indexes)
}

fn call_calsen_reindex(cmd Command) ! {
	dir_list := cmd.flags.get_strings('dir')!
	index := cmd.flags.get_string('index')!
	reindex_files(dir_list, index)
}

[if debug]
fn debug_stuff() {
	c := parse_config_file('./tmp/.calsenconfig')
	println(c.init.config.index_dir)
	println('file_list')
	f_list := get_files_to_index(['./src', './tmp', '/tmp/code_test/calsen-main'])
	println(f_list)

	// for f in f_list {
	// 	file_contents := os.read_file(f) or { panic(err) }
	// 	a := calc_tf_scores_of_file(file_contents_to_tokens(file_contents))
	// 	println(a)
	// }
	println(get_file_parser('./README.md'))
}

fn main() {
	// search command
	mut search_sub_command := Command{
		name: 'search'
		description: 'search the indexed files.'
		execute: call_calsen_search
	}

	search_sub_command.add_flag(Flag{
		flag: .string
		name: 'query'
		abbrev: 'q'
		required: true
		description: 'The query to search the files.'
	})

	search_sub_command.add_flag(Flag{
		flag: .string_array
		name: 'index'
		abbrev: 'i'
		required: false
		default_value: ['default']
		description: 'The index to use.'
	})
	// end search subcommand

	// reindex command
	mut reindex_sub_command := Command{
		name: 'reindex'
		description: 'reindex / index the file that needs to be queried.'
		execute: call_calsen_reindex
	}

	reindex_sub_command.add_flag(Flag{
		flag: .string_array
		name: 'dir'
		abbrev: 'd'
		required: true
		description: 'The directory to index'
	})

	reindex_sub_command.add_flag(Flag{
		flag: .string
		name: 'index'
		abbrev: 'i'
		description: 'The name of the index where the data will be stored.'
		default_value: ['default']
	})
	// end reindex sub command

	mut app := Command{
		name: 'calsen'
		description: 'A search engine for files.'
		commands: [
			search_sub_command,
			reindex_sub_command,
		]
	}

	debug_stuff()
	app.setup()
	app.parse(os.args)
}
