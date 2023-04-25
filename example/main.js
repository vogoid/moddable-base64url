import base64 from "base64url"

const encoded = base64.encode("Hello, world!");
const decoded = base64.decode(encoded);
const string = String.fromArrayBuffer(decoded);
trace(string + "\n");

debugger;
