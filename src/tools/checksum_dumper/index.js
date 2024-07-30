const { write } = require("fs");
const { MongoClient, ServerApiVersion } = require("mongodb");
// Replace the placeholder with your Atlas connection string
const uri = "mongodb://127.0.0.1:27017/";
// Create a MongoClient with a MongoClientOptions object to set the Stable API version
const client = new MongoClient(uri,  {
        serverApi: {
            version: ServerApiVersion.v1,
            strict: true,
            deprecationErrors: true,
        }
    }
);

var fs = require('node:fs/promises');
let fileHandle;

async function write_checksum(crc, name) {
    var buff = Buffer.alloc(64 + 4);
    buff.writeInt32LE(crc, 0);

    var strBuff = name.toString('ascii');
    buff.write(strBuff, 4);
    await fileHandle.write(buff);

}

async function run() {
    fileHandle = await fs.open('checksums.bin', 'w+');
    try {
      // Connect the client to the server (optional starting in v4.7)
      await client.connect();
      // Send a ping to confirm a successful connection
      var cursor = await client.db("QScript").collection("scriptkeys").find({});
      while(await cursor.hasNext()) {
        var item = await cursor.next();
        await write_checksum(item.checksum, item.name);
      }
    } finally {
      // Ensures that the client will close when you finish/error
      await client.close();
    }
  }


  run().catch(console.dir);