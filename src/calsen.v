// the orchestrator

module main

fn reindex_files(dir_list []string, index string) {
	println('Called reindex files with ${dir_list}, index: ${index}')
}

fn search_files(query string, indexes []string) {
	println('Called search files with the query: ${query}, index: ${indexes}')
}
