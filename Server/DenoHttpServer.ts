import * as open from "https://deno.land/x/open@v0.0.6/index.ts";
import * as path from "https://deno.land/std@0.206.0/path/mod.ts";

//console.log("Received args:", Deno.args);
// for (let i = 0; i < Deno.args.length; i++) {
//   console.log(`Args[${i}]: ${Deno.args[i]}`);
// }

const port = parseInt(Deno.args[0])
const launch = Deno.args[1].toLowerCase() === "true";
const serveDir = Deno.args[2].toString();


const options: Deno.ServeOptions = {
  port: port,
  async onListen({ port, hostname }) {
    console.log(`Server started at http://${hostname}:${port}`);
    if (launch) {
      await open.open(`http://localhost:${port}/`);
    }
  },
};
globalThis.Deno.serve(options, async (req: Request): Promise<Response> => {
  const url = new URL(req.url);
  let filePath = serveDir + url.pathname;
  if (filePath === `${serveDir}/`) {
    filePath = `${serveDir}/index.html`;
  }
  const extension = path.extname(filePath).toLowerCase();

  const contentTypeMap: Record<string, string> = {
    ".html": "text/html; charset=utf-8",
    ".js": "application/javascript; charset=utf-8",
    ".mjs": "application/javascript; charset=utf-8",
    ".wasm": "application/wasm",
    ".wat": "text/plain; charset=utf-8",
  };

  const contentType = contentTypeMap[extension] || "application/octet-stream";
  try {
    const data = await Deno.readFile(filePath);
    console.log("✅ 200: " + url);
    return new Response(data, {
      status: 200,
      headers: new Headers({
        "Content-Type": contentType,
      }),
    });
  } catch (error) {
    if (error instanceof Deno.errors.NotFound) {
      console.log("❌ 404: " + url);
      return new Response(null, {
        status: 404,
        statusText: "Not found",
      });
    } else {
      console.log("❌ 500: " + url);
      return new Response(null, {
        status: 500,
        statusText: "Internal server error.",
      });
    }
  }
});
