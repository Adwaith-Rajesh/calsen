module tf_idf

import os

fn load_tf_idf() {}

pub fn dump_tf_idf(data map[string]map[string]f64, filepath string) {
	// this has to be stupid
	os.write_file(filepath, data.str()) or { panic(err) }
}
