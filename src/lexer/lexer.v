// License: GPL v3 Copyright: 2023, Adwaith Rajesh <adwaithrajesh3180@gmail.com>

module lexer

import regex

fn find_token_re(file_contents string, pattern string, shared tokens []string) {
	mut re, err_num, err_msg := regex.regex_base(pattern)
	if err_num != 0 {
		panic('cannot maker regular expression: ${err_msg}')
	}
	lock tokens {
		for i in re.find_all_str(file_contents) {
			tokens << i.to_lower()
		}
	}
}

fn find_words(file_contents string, shared tokens []string) {
}

pub fn file_contents_to_tokens(file_contents string) []string {
	shared tokens := []string{}
	// find all numbers
	num_finder := spawn find_token_re(file_contents, r'[0-9]+', shared tokens)
	// find all words
	word_finder := spawn find_token_re(file_contents, r'[a-zA-Z]+', shared tokens)

	num_finder.wait()
	word_finder.wait()

	t := rlock tokens {
		tokens
	}
	return t
}
