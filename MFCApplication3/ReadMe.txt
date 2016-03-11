================================================================================
    README
===============================================================================

Application implemented using MVC pattern, so it consists of:
- Crawler (model)
- CrawlerView (view)
- CrawlerController

Crawler
----------------------------------------------
Used simplified idea of dynamic parallel architecture, described in paper "Architecture of A 
Scalable Dynamic Parallel WebCrawler with High Speed Downloadable Capability for 
a Web Search Engine".

Idea is to have one central storage (queue) of URLs (Tasks) that would be dynamicly assigned 
to different threads, and each thread will push found URLs to that storage. Also, we keep track
of visited URLs.

Simplifications:
- We pop only one task at a time from queue (to simplify task). It would be more effective to assign 
a set of URLs to one thread and also to push many URLs as a return from thread. But it would require extra storage
for set of URLs in thread.

- Because task requires BFS, we have to have single queue of URLs, rather than separate queue in each 
thread (that could be more productive, because it reduces number of locks)

- Search of links is trivial - just as described in task. We find prefix 'http://' and then take all valid
symbols that can be in URL. For simplicity, we consider all links (including links in headers, described standarts, links to
css and javascript). In real project I would rather used DOM parser to get <a /> nodes and parsed links from <body />.
Also, I would rather verify string extracted from "href" property with regexp (boost).

- Our search of words is trivial. I assume that task was to develop simple architecture of crawler, rather 
than real search. In real search I would also used DOM to extract text from nodes.

- I've done everything using std::string class, rather than in wstring (unicode), for simplicity.

- Status of each Task (URL) was simplified in form of simple enum. It could be scaled to containe more statuses 
(for http errors like 403 forbidden and so one). I only add 404 not found.