// License: GPL v3 Copyright: 2023, Adwaith Rajesh <adwaithrajesh3180@gmail.com>
module tf_idf

import arrays { map_of_counts }
import math { log10 }

type IndexType = map[string]map[string]f64

pub struct TFIDFScore {
	filename string
	score    f64
}

pub fn calc_tf_scores_of_file(tokens []string) map[string]f64 {
	total_token_count := tokens.len
	mut tf_score := map[string]f64{}
	for tok, count in map_of_counts(tokens) {
		tf_score[tok] = f64(count) / f64(total_token_count)
	}
	return tf_score
}

fn calc_idf_score_of_token(index IndexType, token string) f64 {
	n_docs := index.len
	mut t_count := 0.0
	for _, v in index {
		if token in v {
			t_count += 1
		}
	}

	if t_count != 0 {
		return log10(n_docs / t_count)
	}
	return 0.0
}

pub fn calc_tf_idf_score(index IndexType, tokens []string) []string {
	mut files := []TFIDFScore{}

	mut idf_cache := map[string]f64{}

	for filename, tf_scores in index {
		mut file_tf_idf_score := 0.0
		for token in tokens {
			if token in idf_cache {
				file_tf_idf_score += tf_scores[token] * idf_cache[token]
			} else {
				idf_cache[token] = calc_idf_score_of_token(index, token)
				file_tf_idf_score += tf_scores[token] * idf_cache[token]
			}
		}

		if file_tf_idf_score != 0.0 {
			files << TFIDFScore{
				filename: filename
				score: file_tf_idf_score
			}
		}
	}
	files.sort(a.score > b.score)
	mut final_filenames := []string{}
	for f in files {
		final_filenames << f.filename
	}
	return final_filenames
}
