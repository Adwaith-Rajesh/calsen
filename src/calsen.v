// the orchestrator

module main

import indexer { get_files_to_index }
import parsers { get_file_parser }
import lexer { file_contents_to_tokens, find_token_re }
import tf_idf { calc_tf_idf_score, calc_tf_scores_of_file, dump_tf_idf, load_tf_idf }

fn calc_tf_score_for_file(filepath string, shared tf_map map[string]map[string]f64) {
	parser := get_file_parser(filepath) or { return }
	file_contents := parser(filepath)

	mut tf_scores := calc_tf_scores_of_file(file_contents_to_tokens(file_contents))

	lock tf_map {
		tf_map[filepath] = tf_scores.move()
	}
}

fn reindex_files(dir_list []string, index string) {
	shared tf_map := map[string]map[string]f64{}

	file_list := get_files_to_index(dir_list)
	mut threads := []thread{}

	for file in file_list {
		threads << spawn calc_tf_score_for_file(file, shared tf_map)
	}
	threads.wait()

	rlock tf_map {
		dump_tf_idf(tf_map, index)
	}
}

fn search_files(query string, index string) {
	tf_map := load_tf_idf(index)

	shared q_tokens := []string{}
	nums := spawn find_token_re(query, r'[0-9]+', shared q_tokens)
	words := spawn find_token_re(query, r'[a-zA-Z]+', shared q_tokens)
	nums.wait()
	words.wait()

	tokens := lock {
		q_tokens
	}

	tokens.map(it.to_lower())

	files := calc_tf_idf_score(tf_map, tokens)
	for file in files {
		println(file)
	}
}
