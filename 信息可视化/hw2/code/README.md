# vis-components version 2.0.0

## Project setup
```
npm install
```

### Compiles and hot-reloads for development
```
npm run dev
```

### Compiles and minifies for production
```
npm run build
```

### Run your unit tests
```
npm run test:unit
```

### Lints and fixes files
```
npm run lint
```

### 作业说明
- 参考提供的代码完成 ```tree```、```treemap```、```sunburst``` 中的 1 个组件（ **3 选 1** ），完成的内容包括该可视化图表组件的：
  1. 需求文档，格式可参考提供的 ```bar chart``` 需求文档
  2. 可运行的组件代码，结构可参考提供的 ```bar chart``` 代码
  3. 可运行的交互测试页面，具体可参考提供的 ```bar chart``` 测试页面
   
- 推荐使用 VS Code 进行开发
  
- 需提前安装 ```node```。 安装教程请自行 Google 或百度。
  
- 初次运行时，到code目录下，安装依赖的命令为 ```npm install```。该命令会生成一个 ```node_modules``` 文件夹，里面是程序运行需要的各种依赖包。
  > 交作业时需将这个文件夹删掉，否则文件将会非常大。推荐同学们使用 git 管理自己的代码，作业中的```.gitignore``` 文件中已经 ignore 了 ```node_modules```。不会使用 git 的同学如果用压缩包备份作业，也建议不要连同该文件夹一同备份，因为在程序运行前只需 ```npm install``` 即可安装依赖。该文件夹如果直接删除速度较慢，可以使用如下命令
```
npm install rimraf -g    # 安装 rimraf
rimraf node_modules    # 删除 node_modules 文件夹
```

- 运行 ```npm run dev``` 等待若干秒，编译成功后，可以在 http://localhost:2020/ 看到测试页面
  
- 页面左侧有四个图表类型 tab，其中 ```bar-chart``` 即为示例测试页面，另外三个需要同学们自行补充代码，运行后选择相应的 tab 查看结果。目前仅支持了 ```设置图表标题的内容``` 和 ```设置是否显示标题```。
  
- 作业使用的前端框架为 ```Vue.js```，同学们可以通过查看[官方教程](https://cn.vuejs.org/v2/guide/)、[文档](https://cn.vuejs.org/v2/api/)、[菜鸟教程](https://www.runoob.com/vue2/vue-tutorial.html)等了解该框架。一些功能性组件已经给出，需要同学们编写的代码仍以 ```d3``` 和 ```js``` 为主，且集中在少数几个文件中。做作业时，需要修改 or 新增的文件包括但不限于（下面的xxx代表你选择的图表组件）：
  > src/components/xxx/xxx.vue  该文件是图表组件的具体定义和实现文件  
  > tests/setting-rules/ 目录下的配置项  
  > tests/views/xxx/xxx.vue  该文件包含了测试页面中的图表组件 & 配置项 tab

- 文档、代码、测试页面的结构都请参考作业中提供的 bar chart 示例，推荐先写好文档，确定好需求，然后再根据需求文档进行开发和测试。文档尽量描述简洁（不要堆砌字数），功能合理、全面。代码尽量可以实现文档所列功能。
  
- 可视化组件的视觉元素和交互可以参考现有可视化工具库（echart，g2，vega-lite）中的相应示例的视觉元素和交互功能，但**不要**使用他们来绘制图表，要求使用 ```d3.js``` 进行开发
  
- 三个可视化组件参考资料
  > 1. tree：https://observablehq.com/@d3/collapsible-tree
  > 2. treemap：https://observablehq.com/@d3/treemap
  > 3. sunburst：https://observablehq.com/@d3/sunburst

- 遇到问题时，如果找不到解决办法，可以及时询问助教

- 提交方式：学在浙大上传压缩包（注意**不要包含 ```node_modules``` 文件夹**）
  
- 截止时间：2021年 1月 12日 中午12点