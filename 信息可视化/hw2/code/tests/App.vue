<template>
  <div id="app" class="full-match">
    <div class="header">
      <h1 class="title">vis-components</h1>
    </div>
    <div class="app">
      <div class="l-box">
        <div
          class="menu-item"
          :class="{'active': $route.path === item.path}"
          @click="$router.push(item.path)"
          v-for="(item, i) in this.$router.options.routes"
          :key="i"
        >{{item.meta.title}}</div>
      </div>
      <div class="r-box">
        <router-view />
      </div>
    </div>
  </div>
</template>
<script>
export default {
  mounted () {
    console.log(this.$route.path);
  }
};
</script>
<style lang='less'>
@import url("./style/common.less");
#app {
  font-family: Avenir, Helvetica, Arial, sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  padding-top: 60px;
  box-sizing: border-box;
}
</style>
<style lang="less" scoped>
.header {
  position: fixed;
  left: 0;
  top: 0;
  width: 100%;
  height: 60px;
  background: #4099ef;
  box-shadow: 0 0 4px 0 rgb(0, 0, 0, 0.06);
  z-index: 9999;
  .title {
    font-size: 24px;
    color: #fff;
    line-height: 60px;
    margin: 0;
    padding: 0 20px;
  }
}
.app {
  width: 100%;
  height: calc(~"100vh - 60px");
  display: flex;
  flex-flow: row nowrap;
  .l-box {
    display: block;
    width: 200px;
    height: 100%;
    background: #fff;
    border-right: 1px solid rgba(0, 0, 0, 0.05);
  }
  .r-box {
    display: block;
    width: calc(~"100vw - 200px");
    height: 100%;
    background: #fafafa;
  }
  .menu-item {
    width: 100%;
    height: 40px;
    font-size: 14px;
    color: #555;
    line-height: 40px;
    box-sizing: border-box;
    padding-left: 20px;
    cursor: pointer;
    &:hover,
    &.active {
      background: #f5f5ff;
    }
  }
}
</style>
