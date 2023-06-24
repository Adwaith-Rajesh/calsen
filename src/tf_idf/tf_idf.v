// License: GPL v3 Copyright: 2023, Adwaith Rajesh <adwaithrajesh3180@gmail.com>
module tf_idf

import arrays { map_of_counts }

pub fn calc_tf_scores_of_file(tokens []string) map[string]f64 {
	total_token_count := tokens.len
	mut tf_score := map[string]f64{}
	for tok, count in map_of_counts(tokens) {
		tf_score[tok] = f64(count) / f64(total_token_count)
	}
	return tf_score
}
