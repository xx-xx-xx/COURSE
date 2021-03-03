<template>
  <div class="comt-test-grid" :class="{ open: isSettingOpen }">
    <div class="l-box">
      <el-scrollbar style="height:100%">
        <slot name="l-box"></slot>
      </el-scrollbar>
    </div>
    <div class="r-box">
      <div class="handle" @click="toggleSetting">
        <i
          class="icon"
          :class="isSettingOpen ? 'el-icon-arrow-right' : 'el-icon-arrow-left'"
        ></i>
      </div>
      <el-scrollbar style="height:100%">
        <div style="padding:20px">
          <slot name="r-box"></slot>
        </div>
      </el-scrollbar>
    </div>
  </div>
</template>
<script>
/**
 * 测试用布局组件
 */
export default {
  name: 'test-grind',
  data() {
    return {
      isSettingOpen: true,
    };
  },
  methods: {
    toggleSetting() {
      this.isSettingOpen = !this.isSettingOpen;
    },
  },
};
</script>
<style lang="less" scoped>
.comt-test-grid {
  width: 100%;
  height: 100%;
  margin: 0;
  padding: 0;
  display: flex;
  flex-flow: row nowrap;
  justify-self: flex-start;
  max-height: 100vh;
  overflow: hidden;
  &.open {
    .l-box {
      width: 72%;
    }
    .r-box {
      width: 28%;
    }
  }
  .l-box {
    transition: width ease-in 0.3s;
    box-sizing: border-box;
    padding: 20px;
    width: 100%;
    height: 100%;
    display: block;
    position: relative;
    background: #eee;
  }
  .r-box {
    box-sizing: border-box;
    transform-origin: 100% 0;
    transition: width ease-in 0.3s;
    width: 0;
    display: block;
    position: relative;
    height: 100%;
    background: #fff;
    box-shadow: 0 0 4px 0 rgba(0, 0, 0, 0.15);
    .handle {
      position: absolute;
      z-index: 0;
      width: 20px;
      height: 80px;
      border-top-left-radius: 20px;
      border-bottom-left-radius: 20px;
      background: #fff;
      left: 0;
      top: 50%;
      transform: translate(-100%, -50%);
      box-shadow: -2px 0 4px 0 rgba(0, 0, 0, 0.15);
      clip-path: rect(0, 19px, 80px, 0);
      .icon {
        transition: all ease-in 0.3s;
        color: #888;
        position: absolute;
        margin: 0 auto;
        line-height: 80px;
      }
    }
  }
}
</style>
