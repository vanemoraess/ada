#include <iostream>
#include <memory>
#include <cstdlib>
#if ADA_VARIOUS_COMPETITION_ENABLED
#include <uriparser/Uri.h>
#include <EdUrlParser.h>
#include <http_parser.h>
#endif
#if ADA_url_whatwg_ENABLED
#include <src/url.h>
#endif

#include "ada.h"
#include "performancecounters/event_counter.h"
event_collector collector;
size_t N = 1000;

#include <benchmark/benchmark.h>
