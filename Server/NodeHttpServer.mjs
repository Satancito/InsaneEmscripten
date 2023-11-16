import * as http from 'http';
import * as fs from 'fs';
import * as path from 'path';
import * as open from 'open';

//console.log("Received args:", process.argv);
// for (let i = 2; i < process.argv.length; i++) {
//   console.log(`Args[${i - 2}]: ${process.argv[i]}`);
// }

const port = parseInt(process.argv[2])
const launch = process.argv[3].toLowerCase() === "true"
const serveDir = process.argv[4].toString();


const server = http.createServer((req, res) => {
    let filePath = serveDir + req.url;
    if (filePath === `${serveDir}/`) {
        filePath = `${serveDir}/index.html`;
    }
    const url = req.headers.host + req.url;
    const contentTypeMap = new Map([
        ['.html', 'text/html; charset=utf-8'],
        ['.js', 'application/javascript; charset=utf-8'],
        ['.mjs', 'application/javascript; charset=utf-8'],
        ['.wasm', 'application/wasm'],
        ['.wat', 'text/plain; charset=utf-8']
    ]);

    const extension = path.extname(filePath).toLowerCase();
    const contentType = contentTypeMap.get(extension) || 'application/octet-stream';
    fs.readFile(filePath, (error, content) => {
        if (error) {
            if (error.code === 'ENOENT') {
                console.log("❌ 404: "+ url);
                res.writeHead(404);
                res.end('404 Not Found');
            } else {
                console.log("❌ 500: "+ url);
                res.writeHead(500);
                res.end('Error interno del servidor');
            }
        } else {
            console.log("✅ 200: "+ url);
            res.writeHead(200, { 'Content-Type': contentType });
            res.end(content);
        }
    });
});

server.listen(port, async () => {
    console.log(`Server started at http://localhost:${port}/`);
    if(launch)
    {
        await open.default(`http://localhost:${port}`);
    }
});