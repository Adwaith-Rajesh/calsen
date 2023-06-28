module tf_idf

import os
import x.json2

pub fn dump_tf_idf(data map[string]map[string]f64, filepath string) {
	// this has to be stupid
	os.write_file(filepath, data.str()) or { panic(err) }
}

pub fn load_tf_idf(filepath string) map[string]map[string]f64 {
	// there must be an easier way to do it!!
	// I do not want to convert the JSON to struct based thingy
	// I wanted pure maps, so this is it

	tf_idf_json := os.read_file(filepath) or { panic(err) }
	tf_idf_content := json2.raw_decode(tf_idf_json) or {
		eprintln('could not decode index file : ${err}')
		exit(1)
	}

	mut tf_scores := map[string]map[string]json2.Any{}
	mut tf_scores_final := map[string]map[string]f64{}

	// convert JSON string to map[string]json2.Any
	l := tf_idf_content as map[string]json2.Any

	// convert map[string]json2.Any -> map[string]map[string]json2.Any
	for k, v in l {
		tf_scores[k] = (json2.raw_decode(v.str()) or { panic(err) }) as map[string]json2.Any
	}

	// convert map[string]map[string]json2.Any -> map[string]map[sting]f64
	for k, v in tf_scores {
		for ki, vi in v {
			tf_scores_final[k][ki] = vi.f64()
		}
	}

	return tf_scores_final
}
