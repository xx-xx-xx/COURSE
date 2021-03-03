<template>
  <div
    id="tree-container"
    :style="{width:600,height:600}"
  >
  </div>
</template>
<script>
import * as d3 from 'd3';
import * as d3h from 'd3-hierarchy';
import originData from './data.json';

export default {
    name: 'Tree',
    props:['options'],
    data(){
        return {
            tree: null,
            g: null,
            chart: null,
            title: null,
            titleText: 'Tree',
            titleRectWidth: 460,
            titleRectHeight: 40,
            width:460,
            height:460,
            chartPadding : {top: 80, right: 80, bottom: 80, left: 80},
            // add
            zoomsvg: null,
            tooltip: null,
            click: null,
            duration: 750,
            gLinkStroke: { color: '#000', opacity: 0.8, width: 2 },
            text: { fontSize: 15, fontFamily: 'Arial', fontColor: '#000' },
            node: {
                size: 5,
                borderWidth: 2,
                nonLeafNodeFill: '#FFFFFF',
                leafNodeFill: '#80C67D',
            },
            tooltipPadding : {top: 5, right: 5, bottom: 5, left: 5},
            treeLayout: { width: 25, height: 300, direction: 'left' },
            treeRoot: null,
            // data: [],
        };
    },
    // https://cn.vuejs.org/v2/api/#computed
    // https://cn.vuejs.org/v2/guide/computed.html#%E5%9F%BA%E7%A1%80%E4%BE%8B%E5%AD%90
    // computed: {
    //     // 这里是一些计算属性，当其中涉及的值发生变化时，计算属性会重新计算，返回新的值
    //     ascendingData(){ 
    //         // 升序排序
    //         return this.sortKeyAscending(this.originData,'value');
    //     },
    //     descendingData(){
    //         // 降序排序
    //         return this.sortKeyDescending(this.originData,'value');
    //     }
    // },
    // https://cn.vuejs.org/v2/api/#watch
    watch: {
        // watch的作用可以监控一个值的变换，并调用因为变化需要执行的方法。可以通过watch动态改变关联的状态。
        // 这里是一些可被修改的配置项，例如图表标题内容、标题是否显示等
        // title
        'options.titleText': {
            handler(){
                this.titleText = this.options.titleText;
                this.title.select('text').text(this.titleText);
            },
        },
        'options.titleIsShow': {
            handler() {
                if(this.options.titleIsShow){
                    this.title.attr('style','display: block');
                }else{
                    this.title.attr('style','display: none');
                }
            },
        },
        'options.titlePosition': {
          handler() {
            this.updateTitle();
            },
        },
        'options.titleBackground': {
            handler() {
                this.title
                .select('rect')
                .attr('fill', this.options.titleBackground);
            },
        },
        'options.titleFontColor': {
            handler() {
                this.title
                .select('text')
                .attr('fill', this.options.titleFontColor);
            },
        },
        // 修改text相对标题rect的位置,来更改文本对齐方式
        'options.titleTextPosition': {
        handler() {  
            switch (this.options.titleTextPosition) {
            case 'center':
                this.title
                .select('text')
                .attr('text-anchor', 'middle')
                .attr('x', this.titleRectWidth/2);
                break;
            case 'left':
                this.title
                .select('text')
                .attr('text-anchor', 'start')
                .attr('x', 10);
                break;
            case 'right':
                this.title
                .select('text')
                .attr('text-anchor', 'end')
                .attr('x', this.titleRectWidth-10);
                break;
            default:
                break;
            }
        },
        },
        'options.titleFontFamily': {
        handler() {
            this.title
            .select('text')
            .attr('font-family', this.options.titleFontFamily);
        },
        },
        'options.titleFontSize': {
        handler() {
            this.title
            .select('text')
            .attr('font-size', this.options.titleFontSize);
        },
        },
        // 请根据组件需要补充...
        // leafnode fill
        'options.nonLeafNodeFill': {
        handler() {
            this.node.nonLeafNodeFill = `${this.options.nonLeafNodeFill}`;
            this.gNode
            .selectAll('circle')
            .attr('fill', (d) =>
                d._children
                ? `${this.node.nonLeafNodeFill}`
                : `${this.node.leafNodeFill}`
            );
        },
        },
        'options.leafNodeFill': {
        handler() {
            this.node.leafNodeFill = `${this.options.leafNodeFill}`;
            this.gNode
            .selectAll('circle')
            .attr('fill', (d) =>
                d._children
                ? `${this.node.nonLeafNodeFill}`
                : `${this.node.leafNodeFill}`
            );
        },
        },
        'options.nodeSize': {
          handler() {
            this.node.size = this.options.nodeSize;
            this.gNode.selectAll('circle').attr('r', this.node.size);
          },
        },
        'options.nodeBorderWidth': {
          handler() {
            this.node.borderWidth = this.options.nodeBorderWidth;
            this.gNode
              .selectAll('circle')
              .attr('stroke-width', this.node.borderWidth);
          },
        },
        // text
        'options.textIsShow': {
        handler() {
            if (this.options.textIsShow) {
            this.gNode.selectAll('text').attr('style', 'display: block');
            } else {
            this.gNode.selectAll('text').attr('style', 'display: none');
            }
        },
        },
        'options.textFontFamily': {
        handler() {
            this.text.fontFamily = this.options.textFontFamily;
            this.gNode
            .selectAll('text')
            .attr('font-family', this.text.fontFamily);
        },
        },
        'options.textFontColor': {
        handler() {
            this.text.fontColor = `${this.options.textFontColor}`;
            this.gNode.selectAll('text').attr('fill', this.text.fontColor);
        },
        },
        // linkstroke
        'options.linkStrokeColor': {
        handler() {
            this.gLinkStroke.color = `${this.options.linkStrokeColor}`;
            this.gLink.attr('stroke', this.gLinkStroke.color);
        },
        },
        'options.linkStrokeOpacity': {
        handler() {
            this.gLinkStroke.opacity = this.options.linkStrokeOpacity / 10;
            this.gLink.attr('stroke-opacity', this.gLinkStroke.opacity);
        },
        },
        'options.linkStrokeWidth': {
        handler() {
            this.gLinkStroke.width = this.options.linkStrokeWidth;
            this.gLink.attr('stroke-width', this.gLinkStroke.width);
        },
        },
        // tooltip
        'options.tooltipIsShow': {
        handler(){
            if(this.options.tooltipIsShow){
            this.tooltip.attr('style', 'display: block');
            }else{
            this.tooltip.attr('style', 'display: none');
            }
        }
        },
        'options.tooltipColor': {
        handler() {
            this.tooltip.select('rect')
            .attr('fill', this.options.tooltipColor);
        }
        },
        'options.tooltipBorder': {
        handler() {
            this.tooltip.select('rect')
            .attr('style', `stroke-width: ${this.options.tooltipBorder}`);
        }
        },
        'options.tooltipBorderRadius': {
        handler() {
            this.tooltip.select('rect')
            .attr('rx', `${this.options.tooltipBorderRadius}`)
            .attr('ry', `${this.options.tooltipBorderRadius}`);
        }
        },
    },
    // https://cn.vuejs.org/v2/api/#mounted
    mounted() {
        // 这里会在实例被挂载后调用
        // 初始化图表
        this.initTree();
    },
    // https://cn.vuejs.org/v2/api/#methods
    methods: {
        // 这里是一些组件其余地方会使用到的函数，调用方式为 this.xxxx()
        // 例如mounted中的图表初始化函数 initTree() 就应该被定义在这里
        // 其余与交互、更新有关的函数也都写在这里
        initTree() {
          // 在这里编写初始化图表的代码，以下代码仅供参考，均可调整
          // 可以使用d3绘制可视化图表，具体可参考 bar chart 示例和 README.md 中的链接
          // console.log(this.options);

          // 指定图表的宽高
          this.width = 700 - this.chartPadding.right - this.chartPadding.left-180;
          this.height = 700 -this.chartPadding.bottom - this.chartPadding.top-80;
          
          d3.select('#tree-container')
                .style('width','800px')
                .style('height','720px');
          
          // 添加svg
          this.svg = d3.select('#tree-container').append('svg')
                          .attr('style','background: #eee')
                          // 自适应区域大小
                          .attr('preserveAspectRatio', 'xMidYMid meet')
                          .attr('viewBox','0 0 900 900')
                          // 添加zoom事件回调函数
                          .call(d3.zoom().on('zoom', () => {
                              console.log(d3.event.transform);
                              this.zoomsvg.attr('transform', d3.event.transform);
                              })
                          );
          // 添加zoomsvg，用于zoom变换
          this.zoomsvg = this.svg.append('g');

          // 添加g标签 
          this.g = this.zoomsvg.append('g')
          .attr('transform',`translate(${this.chartPadding.left+40}, ${this.chartPadding.top+40})`);
          // .attr('transform', `translate(${this.width / 2},${this.height})`);
          //               .attr('class','chart')  // 图表部分
          //               .attr('transform',`translate(${this.chartPadding.left+40}, ${this.chartPadding.top+40})`);

          // 添加图表标题
          this.title = this.svg.append('g')
                          .attr('transform','translate(0,0)')
                          .attr('style','display: none');     // 默认不显示
          // 标题背景框
          this.title.append('rect')
                  .attr('class','title')
                  .attr('width', 700)
                  .attr('height',`${this.titleRectHeight}`)
                  .attr('fill','#E3E3E3')
                  .attr('x','0')
                  .attr('y','0');
          // 标题文本       
          this.title.append('text')
                  .text(this.titleText)
                  .attr('x',350)
                  .attr('y',25)
                  .attr('text-anchor','middle')
                  .attr('fill','#000');

          // 添加links
          this.gLink = this.g
              .append('g')
              .attr('fill', 'none')
              .attr('stroke', this.gLinkStroke.color)
              .attr('stroke-opacity', this.gLinkStroke.opacity)
              .attr('stroke-width', this.gLinkStroke.width);
          // 添加nodes
          this.gNode = this.g
              .append('g')
              .attr('cursor', 'pointer')
              .attr('pointer-events', 'all');
          // 加载数据
          // 处理数据得到树根treeRoot
          this.treeRoot = d3h.hierarchy(originData);
          this.treeRoot.x0 = this.treeLayout.width / 2;
          this.treeRoot.y0 = 0;
          this.treeRoot.descendants().forEach((d, i) => {
              d.id = i;
              d._children = d.children;
              if (d.depth>=2 || (d.depth && d.data.name.length !== 7 )) d.children = null;
          });
          // 初始化tree
          this.tree = d3.tree();
          this.tree.nodeSize([this.treeLayout.width, this.treeLayout.height]);
          this.tree(this.treeRoot);
          this.updateTree(this.treeRoot);
      
          // 添加tooltip
          this.tooltip = this.g
              .append('g')
              .attr('class', 'tooltip')
              .attr('opacity', 0);
          // 添加矩形框
          this.tooltip.append('rect')
              .attr('fill', '#eeeeee')
              .attr('width', 150)
              .attr('height', 23)
              .attr('rx', 0)
              .attr('ry', 0)
              .attr('stroke', 'black')
              .attr('style', 'stroke-width:1');
          // 添加文本
          this.tooltip.append('text')
              .attr('font-size',12)
              .attr('font-color', '#000')
              .attr('transform', `translate(${this.tooltipPadding.left},${this.tooltipPadding.top+12})`);
        },

    updateTree(source) {
      // 水平连线
      this.linkH = d3.linkHorizontal().x((d) => d.y).y((d) => d.x);
      this.g.attr('writing-mode', 'horizontal-tb');
      const nodes = this.treeRoot.descendants().reverse();
      const links = this.treeRoot.links();
      // Compute the new tree layout
      this.tree(this.treeRoot);

      // transition setting
      const transition = this.g.transition().duration(this.duration);

      // Update the nodes…
      const node = this.gNode.selectAll('g').data(nodes, (d) => d.id);

      // Enter any new nodes at the parent's previous position.
      const nodeEnter = node.enter().append('g')
        .attr('transform', `translate(${source.y0},${source.x0})`)
        .attr('fill-opacity', 0)
        .attr('stroke-opacity', 0)
        .on('click', (d) => {
          if (d3.event.defaultPrevented) return; // click suppressed
          d.children = d.children ? null : d._children;
          this.updateTree(d);
        });

      // new node
      nodeEnter.append('circle')
        .attr('class', 'node')
        .attr('r', this.node.size)
        .attr('fill', (d) =>
          d._children ? this.node.nonLeafNodeFill : this.node.leafNodeFill
        )
        .attr('stroke', this.color)
        .attr('stroke-width', this.node.borderWidth);
      // new node text
      nodeEnter.append('text')
        .attr('x', (d) => (d._children ? -6 : 6))
        .attr('dy', '.31em')
        .attr('class', 'nodeText')
        .attr('text-anchor', (d) => (d._children ? 'end' : 'start'))
        .text((d) => d.data.name)
        .attr('font-size', this.text.fontSize)
        .attr('font-family', this.text.fontFamily)
        .attr('fill', this.text.fontColor)
        .clone(true).lower()
        .attr('stroke-linejoin', 'round')
        .attr('stroke-width', 3)
        .attr('stroke', 'white');
      // 对节点的文本添加mouseover和mouseout事件
      // 显示tooltip
      this.gNode.selectAll('text')
        .on('mouseover', function(d){
          if(! d._children){
            const w = this.getBBox().width*(d.data.name.length+7)/d.data.name.length;
            const x = d.y0 + w;
            const y = d.x0 - 12;
            d3.select('.tooltip')
              .attr('transform', `translate(${x},${y})`)
              .attr('opacity', 0.7);
            d3.select('.tooltip').select('rect')
              .attr('width', w);
            d3.select('.tooltip').select('text')
              .text(`(${d.data.name}: ${d.data.value})`);
          }
        })
        .on('mouseout', function(d){
          if(! d._children){
            d3.select('.tooltip')
              .attr('opacity',0);
          }
        });
      // 控制节点文本的显示
      if (this.options.textIsShow) {
        this.gNode.selectAll('text').attr('style', 'display: block');
      } else {
        this.gNode.selectAll('text').attr('style', 'display: none');
      }
      // Transition nodes to their new position.
      node.merge(nodeEnter).transition(transition)
        .attr('transform', (d) => `translate(${d.y},${d.x})`)
        .attr('fill-opacity', 1)
        .attr('stroke-opacity', 1);

      // Transition exiting nodes to the parent's new position.
      node.exit().transition(transition).remove()
        .attr('transform', `translate(${source.y0},${source.x0})`)
        .attr('fill-opacity', 0)
        .attr('stroke-opacity', 0);

      // Update the links...
      const link = this.gLink.selectAll('path').data(links, (d) => d.target.id);

      // Enter any new links at the parent's previous position.
      const linkEnter = link.enter().append('path')
        .attr('d', () => {
          const o = { x: source.x0, y: source.y0 };
          return this.linkH({ source: o, target: o });
        });

      // Transition links to their new position.
      link.merge(linkEnter).transition(transition)
        .attr('d', this.linkH);

      // Transition exiting nodes to the parent's new position.
      link.exit().transition(transition).remove()
        .attr('d', () => {
          const o = { x: source.x, y: source.y };
          return this.linkH({ source: o, target: o });
        });

      // Stash the old positions for transition.
      nodes.forEach((d) => {
        d.x0 = d.x;
        d.y0 = d.y;
      });
    },

    // 根据节点，映射一种颜色
    color(node){
      const color = d3.scaleOrdinal()
        .domain(this.treeRoot.descendants()
          .filter(d => d.depth<=1).map(d => d.data.name))
        .range(d3.schemeCategory10);
      if(node.depth===0){
        return color(node.data.name);
      }
      while(node.depth > 1){
        node = node.parent;
      }
      return color(node.data.name);
    }
   },
};
</script>
<style scoped>
text {
  background: "#000";
}
#tree-container {
  overflow: hidden;
}
</style>