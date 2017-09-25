# Url Shortener with Libevent and leveldb
an Experimental project to implement url shortener with key-value db. written in C. see https://upil.in in action

### External
external sources :

* ![Leveldb](https://github.com/google/leveldb)
* ![Snappy](https://github.com/google/snappy)
* ![cJSON](https://github.com/DaveGamble/cJSON)
* ![Libevent](https://github.com/nmathewson/Libevent)



### Compile
```bash
make
```

### Run

#### 1. Run the service
```bash
./shortensvc -h [host] -p [port] -n [name] -i [html for index]
./shortensvc -h 0.0.0.0 -p 10000 -n shortener -i src/html/index.html
```
#### 2 Open in browser

* Open in your browser http://localhost:10000
* Submit custom or random url

### nginx integration

If you are running you website on port ```80``` with domain ```hello.com```. you can use nginx reverse proxy

```
server {
    listen 80;
    server_name hello.com www.hello.com;

	location / {
        proxy_set_header X-Real-IP $remote_addr;
        proxy_set_header   Host      $http_host;
        proxy_pass         http://127.0.0.1:10000;
    }
}
```

Then you can access this url shortener on your domain

# License

MIT License

Copyright (c) [2017] [Arif Nur Khoirudin]

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
