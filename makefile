CXX ?= g++


DEBUG ?= 1
ifeq ($(DEBUG), 1)
    CXXFLAGS += -g
# else
#     CXXFLAGS += -O2

endif
GAME_SRCS := $(wildcard ./gamebox/*.cpp)
WS_SRCS := $(wildcard ./wsocket/*cpp)

server: main.cpp  ./timer/lst_timer.cpp ./http/http_conn.cpp ./http/setfd.cpp ./log/log.cpp ./CGImysql/sql_connection_pool.cpp  webserver.cpp config.cpp $(GAME_SRCS) $(WS_SRCS)
	$(CXX) -o server  $^ $(CXXFLAGS) -lpthread -lmysqlclient -lssl -lcrypto

clean:
	rm  -r server
