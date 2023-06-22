// License: GPL v3 Copyright: 2023, Adwaith Rajesh <adwaithrajesh3180@gmail.com>

module indexer

import os

fn get_files_to_index_from_dir(dir string, shared file_list []string) {
	os.walk(dir, fn [mut file_list] (filename string) {
		lock file_list {
			file_list << filename
		}
	})
}

pub fn get_files_to_index(dir_list []string) []string {
	shared file_list := []string{}

	mut threads := []thread{}
	for dir in dir_list {
		threads << spawn get_files_to_index_from_dir(dir, shared file_list)
	}
	threads.wait()
	f_list := lock file_list {
		file_list // convert shared []string{} to []string{}
	}
	return f_list
}
