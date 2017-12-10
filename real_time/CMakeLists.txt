cmake_minimum_required(VERSION 3.8)
project(real_time)
set(CURL_LIBRARY "-lcurl")
set(CMAKE_CXX_STANDARD 11)

find_library(TINS_LIBRARY tins)

set(SOURCE_FILES
        include/cpr/api.h
        include/cpr/auth.cpp
        include/cpr/auth.h
        include/cpr/body.h
        include/cpr/cookies.cpp
        include/cpr/cookies.h
        include/cpr/cpr.h
        include/cpr/cprtypes.cpp
        include/cpr/cprtypes.h
        include/cpr/curlholder.h
        include/cpr/defines.h
        include/cpr/digest.cpp
        include/cpr/digest.h
        include/cpr/error.cpp
        include/cpr/error.h
        include/cpr/low_speed.h
        include/cpr/max_redirects.h
        include/cpr/multipart.cpp
        include/cpr/multipart.h
        include/cpr/parameters.cpp
        include/cpr/parameters.h
        include/cpr/payload.cpp
        include/cpr/payload.h
        include/cpr/proxies.cpp
        include/cpr/proxies.h
        include/cpr/response.h
        include/cpr/session.cpp
        include/cpr/session.h
        include/cpr/ssl_options.cpp
        include/cpr/ssl_options.h
        include/cpr/timeout.cpp
        include/cpr/timeout.h
        include/cpr/util.cpp
        include/cpr/util.h
        include/elasticsearch/elasticsearch.cpp
        include/elasticsearch/elasticsearch.h
        include/elasticsearch/json_response.cpp
        include/elasticsearch/json_response.h
        include/rapidjson/error/en.h
        include/rapidjson/error/error.h
        include/rapidjson/internal/biginteger.h
        include/rapidjson/internal/diyfp.h
        include/rapidjson/internal/dtoa.h
        include/rapidjson/internal/ieee754.h
        include/rapidjson/internal/itoa.h
        include/rapidjson/internal/meta.h
        include/rapidjson/internal/pow10.h
        include/rapidjson/internal/regex.h
        include/rapidjson/internal/stack.h
        include/rapidjson/internal/strfunc.h
        include/rapidjson/internal/strtod.h
        include/rapidjson/internal/swap.h
        include/rapidjson/msinttypes/inttypes.h
        include/rapidjson/msinttypes/stdint.h
        include/rapidjson/allocators.h
        include/rapidjson/cursorstreamwrapper.h
        include/rapidjson/document.h
        include/rapidjson/encodedstream.h
        include/rapidjson/encodings.h
        include/rapidjson/filereadstream.h
        include/rapidjson/filewritestream.h
        include/rapidjson/fwd.h
        include/rapidjson/istreamwrapper.h
        include/rapidjson/memorybuffer.h
        include/rapidjson/memorystream.h
        include/rapidjson/ostreamwrapper.h
        include/rapidjson/pointer.h
        include/rapidjson/prettywriter.h
        include/rapidjson/rapidjson.h
        include/rapidjson/reader.h
        include/rapidjson/schema.h
        include/rapidjson/stream.h
        include/rapidjson/stringbuffer.h
        include/rapidjson/writer.h
        lib/libnetstat/libnetstat.cpp
        lib/libnetstat/libnetstat.h
        config.yml
        main.cpp
        Makefile
        sniffer.cpp
        sniffer.h
        include/elasticsearch/BulkBody.cpp
        include/elasticsearch/BulkBody.h)

set(CURL_LIBRARY "-lcurl")
find_package(CURL REQUIRED)
add_executable(real_time ${SOURCE_FILES})
include_directories(${CURL_INCLUDE_DIR})
target_link_libraries(real_time ${CURL_LIBRARIES} ${TINS_LIBRARY})

