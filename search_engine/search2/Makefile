all: crawl search-shell

search-shell: search-shell.cpp handler.o txt_parser.o util.o webpage.o searcheng.o md_parser.o cmdhandler.o searchui.o combiners.o
	g++ -g -Wall -std=c++11 $^ -o $@

crawl: crawl.cpp md_parser.o txt_parser.o util.o webpage.o
	g++ -g -Wall -std=c++11 $^ -o $@

md_parser.o: md_parser.cpp md_parser.h pageparser.h
	g++ -g -Wall -std=c++11 -c md_parser.cpp -o md_parser.o

txt_parser.o: txt_parser.cpp txt_parser.h pageparser.h 
	g++ -g -Wall -std=c++11 -c txt_parser.cpp -o txt_parser.o

util.o: util.cpp util.h 
	g++ -g -Wall -std=c++11 -c util.cpp -o util.o

webpage.o: webpage.cpp webpage.h 
	g++ -g -Wall -std=c++11 -c webpage.cpp -o webpage.o

combiners.o: combiners.cpp wpscombiner.h 
	g++ -g -Wall -std=c++11 -c combiners.cpp -o combiners.o

cmdhandler.o: cmdhandler.cpp cmdhandler.h 
	g++ -g -Wall -std=c++11 -c cmdhandler.cpp -o cmdhandler.o

handler.o: handler.cpp handler.h
	g++ -g -Wall -std=c++11 -c  handler.cpp -o handler.o

searcheng.o: searcheng.cpp searcheng.h
	g++ -g -Wall -std=c++11 -c searcheng.cpp -o searcheng.o

searchui.o: searchui.cpp searchui.h
	g++ -g -Wall -std=c++11 -c searchui.cpp -o searchui.o
