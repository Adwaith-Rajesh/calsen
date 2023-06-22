// the orchestrator

module main

fn reindex_file(dir_list []string) {
	println('Called reindex files with ${dir_list}')
}

fn search_files(query string, indexes []string) {
	println('Called search files with the query: ${query}, index: ${indexes}')
}
