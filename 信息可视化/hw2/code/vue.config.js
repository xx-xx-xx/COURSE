const path = require("path");

function resolve(dir) {
  return path.join(__dirname, dir);
}
console.log('current env=>', process.env.NODE_ENV);
module.exports = {
  "devServer": {
    "port": 2020 // 端口
  },
  "chainWebpack": options => {
    options.when(process.env.NODE_ENV === "development", config => {
      config
        .entry("app")
        .clear()
        .add("./tests/main.ts");
      options.resolve.alias.set("@/", resolve("src/"));
      options.resolve.alias.set("~/tests", resolve("tests/"));
    });
    options.when(process.env.NODE_ENV === "production", config => {
      config
        .entry("app")
        .clear()
        .add("./src/index.ts");
    });
  }
};