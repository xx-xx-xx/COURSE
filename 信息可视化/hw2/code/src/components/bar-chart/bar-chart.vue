<template>
    <div id="bar-chart-container" :style="{width:600,height:600}" >
    </div>
</template>
<script>
import * as d3 from 'd3';


export default {
    name: 'BarChart',
    props:['options'],
    data(){
        return {
            barChart: null,
            g: null,
            chart: null,
            title: null,
            legend: null,
            tooltip: null,
            legendBox: null,
            brush: null,
            gBrush:null,
            legendWidth: 80,
            legendHeight: 170,
            titleText: 'Simple Bar Chart',
            titleRectWidth: 460,
            titleRectHeight: 40,
            xAxis: null,
            yAxis: null,
            xscale: null,
            yscale: null,
            nameArray: null,
            valueArray: null,
            width:460,
            height:460,
            chartPadding : {top: 80, right: 80, bottom: 80, left: 80},
            tooltipPadding : {top: 5, right: 5, bottom: 5, left: 5},
            originData: [
                {name: '美国', value: 2722, color: '#BCA2A2'},
                {name: '意大利', value: 2150, color: '#E0B09F'},
                {name: '俄罗斯', value: 1612, color: '#CAD9B0'},
                {name: '巴西', value: 1474, color: '#B0C4D9'},
                {name: '英国', value: 1800, color: '#C6B0D9'},
                {name: '其他', value: 2233, color: '#F2DEAA'}
            ],
            data: [
                {name: '美国', value: 2722, color: '#BCA2A2'},
                {name: '意大利', value: 2150, color: '#E0B09F'},
                {name: '俄罗斯', value: 1612, color: '#CAD9B0'},
                {name: '巴西', value: 1474, color: '#B0C4D9'},
                {name: '英国', value: 1800, color: '#C6B0D9'},
                {name: '其他', value: 2233, color: '#F2DEAA'}
            ],
            colorList:['#BCA2A2','#E0B09F','#CAD9B0','#B0C4D9','#C6B0D9','#F2DEAA']
        };
    },
    // https://cn.vuejs.org/v2/api/#mounted
    mounted() {
        // 这里会在实例被挂载后调用
        // 初始化图表
        this.initBarChart();
    },
    // https://cn.vuejs.org/v2/api/#computed
    // https://cn.vuejs.org/v2/guide/computed.html#%E5%9F%BA%E7%A1%80%E4%BE%8B%E5%AD%90
    computed: {
        // 这里是一些计算属性，当其中涉及的值发生变化时，计算属性会重新计算，返回新的值
        ascendingData(){ 
            // 升序排序
            return this.sortKeyAscending(this.originData,'value');
        },
        descendingData(){
            // 降序排序
            return this.sortKeyDescending(this.originData,'value');
        }
    },
    // https://cn.vuejs.org/v2/api/#watch
    watch: { 
        // watch的作用可以监控一个值的变换，并调用因为变化需要执行的方法。可以通过watch动态改变关联的状态。
        // 这里是一些可被修改的配置项，例如图表标题内容、标题是否显示等
        'options.titleText': {
            handler(){
                this.titleText = this.options.titleText;
                this.title.select('text').text(this.titleText);
            }
        },
        'options.titleIsShow': {
            handler() {
                if(this.options.titleIsShow){
                    this.title.attr('style','display: block');
                }else{
                    this.title.attr('style','display: none');
                }
            }
        },
        'options.titlePosition': {
            handler() {
                this.updateTitle();
            }
        },
        'options.titleBackground': {
            handler() {
                if(this.options.titleIsShow){
                   this.title.select('rect').attr('fill',`${this.options.titleBackground}`);
                }
            }
        },
        'options.titleFontColor': {
            handler() {
                if(this.options.titleIsShow){
                   this.title.select('text').attr('fill',`${this.options.titleFontColor}`);
                }
            }
        },
        'options.titleTextPosition': {
            handler() {
                if(this.options.titleIsShow){
                    // 修改text相对标题rect的位置,来更改文本对齐方式
                    switch(this.options.titleTextPosition){
                        case 'center':  this.title.select('text')
                                            .attr('text-anchor','middle')
                                            .attr('x',350);
                                        break;
                        case 'left':    this.title.select('text')
                                            .attr('text-anchor','start')
                                            .attr('x',10);
                                        break;
                        case 'right':   this.title.select('text')
                                            .attr('text-anchor','end')
                                            .attr('x',690);
                                        break;
                        default: break;
                    }
                    
                }
            }
        },
        'options.titleFontFamily': {
            handler() {
                if(this.options.titleIsShow){
                   this.title.select('text').attr('font-family',`${this.options.titleFontFamily}`);
                }
            }
        },
        'options.titleFontSize': {
            handler() {
                if(this.options.titleIsShow){
                   this.title.select('text').attr('font-size',`${this.options.titleFontSize}`);
                }
            }
        },
        'options.legendIsShow': {
            handler() {
                if(this.options.legendIsShow){
                    this.legendBox.attr('style','display: block');
                }else{
                    this.legendBox.attr('style','display: none');
                }
            }
        },
        'options.legendPosition': {
            handler() {
                if(this.options.legendIsShow){
                    this.updateLegendPosition(); // 更新图例位置
                }
            }
        },
        'options.legendAlign': {
            handler() {
                if(this.options.legendIsShow){
                    if(this.options.legendAlign==='vertical'){
                        // 垂直排列
                        this.legendWidth=80;
                        this.legendHeight=170;;
                        this.legend = this.legendBox.selectAll('.legend') 
                                .attr('transform', function(d, i) { return (`translate(10, ${i * 25})`); });
                    }else{
                        // 水平排列
                        this.legendWidth=430;
                        this.legendHeight=40;
                        this.legend = this.legendBox.selectAll('.legend') 
                                .attr('transform', function(d, i) { return (`translate(${i * 70},0)`); });
                    }
                    // 更新图例位置
                    this.updateLegendPosition();
                }
            }
        },
        'options.xAxisIsShow': {
            handler() {
                if(this.options.xAxisIsShow){
                   this.xAxis.attr('style','display: block');
                }else{
                    this.xAxis.attr('style','display: none');
                }
            }
        },
        'options.xAxisPosition': {
            handler() {
                if(this.options.xAxisIsShow){
                    let ys = this.yscale;
                    if(this.options.xAxisPosition==='top') {
                        this.xAxis.attr('transform','translate(0, 0)').call(d3.axisTop(this.xscale));
                        this.xAxis.selectAll('.tick').selectAll('.axis-label')
                                    .attr('transform','translate(0,-5)');
                        // 更新y轴--当x轴载上方时,y轴要进行反转
                        this.updateYAxis();
                        // 所有bar上沿对齐
                        this.chart.selectAll('.bar').attr('y', 0);
                        
                    }else{
                        this.xAxis.attr('transform',`translate(0, ${this.height})`).call(d3.axisBottom(this.xscale));
                        this.xAxis.selectAll('.tick').selectAll('.axis-label')
                                    .attr('transform','translate(0,0)');
                        // 更新y轴
                        this.updateYAxis();
                        ys = this.yscale;
                        // 所有bar下沿对齐
                        this.chart.selectAll('.bar').attr('y', function (item) {
                            return ys(item.value);
                        });
                    }
                    if(this.options.yAxisPosition==='left')
                        this.yAxis.attr('transform','translate(0, 0)').call(d3.axisLeft(this.yscale).ticks(6));
                    else
                        this.yAxis.attr('transform',`translate(${this.width},0)`).call(d3.axisRight(this.yscale).ticks(6));

                    // 更新bar对应的数据标签位置
                    this.updateDataLabel();
                    
                }
            }
        },
        'options.xAxisLabelIsShow': {
            handler() {
               if(this.options.xAxisIsShow){
                    if(this.options.xAxisLabelIsShow){
                        this.xAxis.selectAll('.tick').selectAll('.axis-label').attr('style', 'display: block');
                    }
                    else{
                        this.xAxis.selectAll('.tick').selectAll('.axis-label').attr('style', 'display: none');
                    }
                }
            }
        },
        'options.xAxisTickIsShow': {
            handler() {
               if(this.options.xAxisIsShow){
                    if(this.options.xAxisTickIsShow){
                        this.xAxis.selectAll('.tick').selectAll('.axis-tick').attr('style', 'display: block');
                    }
                    else{
                        this.xAxis.selectAll('.tick').selectAll('.axis-tick').attr('style', 'display: none');
                    }
                }
            }
        },
        'options.xAxisGridIsShow': {
            handler() {
                this.updateGrid();
                if(this.options.xAxisGridIsShow){
                    this.xAxis.selectAll('.grid').attr('style', 'display: block');
                     
                }else{
                    this.xAxis.selectAll('.grid').attr('style', 'display: none');
                }
            }
        },
        'options.yAxisIsShow': {
            handler() {
                if(this.options.yAxisIsShow){
                   this.yAxis.attr('style','display: block');
                }else{
                    this.yAxis.attr('style','display: none');
                }
            }
        },
        'options.yAxisTickIsShow': {
            handler() {
                if(this.options.yAxisIsShow){
                    if(this.options.yAxisTickIsShow){
                        this.yAxis.selectAll('.tick').selectAll('.axis-tick').attr('style', 'display: block');
                    }
                    else{
                        this.yAxis.selectAll('.tick').selectAll('.axis-tick').attr('style', 'display: none');
                    }
                }
            }
        },
        'options.yAxisLabelIsShow': {
            handler() {
               if(this.options.yAxisIsShow){
                    if(this.options.yAxisLabelIsShow){
                        this.yAxis.selectAll('.tick').selectAll('.axis-label').attr('style', 'display: block');
                    }
                    else{
                        this.yAxis.selectAll('.tick').selectAll('.axis-label').attr('style', 'display: none');
                    }
                }
            }
        },
        'options.yAxisPosition': {
            handler() {
                if(this.options.yAxisIsShow){
                   if(this.options.yAxisPosition==='left') {
                        this.yAxis.attr('transform','translate(0, 0)').call(d3.axisLeft(this.yscale).ticks(6));
                        this.yAxis.selectAll('.tick').selectAll('.axis-label').attr('text-anchor','end');
                        this.yAxis.selectAll('.grid').attr('x2', this.width);
                   }else{
                        this.yAxis.attr('transform',`translate(${this.width},0)`).call(d3.axisRight(this.yscale).ticks(6));
                        this.yAxis.selectAll('.tick').selectAll('.axis-label').attr('text-anchor','start');
                        this.yAxis.selectAll('.grid').attr('x2', -this.width);
                   }
                   
                }
            }
        },
        'options.yAxisScale': {
            handler() {
                this.updateYAxis();
            }
        },
        'options.yAxisNice': {
            handler() {
                this.updateYAxis();
            }
        },
        'options.yAxisGridIsShow': {
            handler() {
                this.updateGrid();
                if(this.options.yAxisGridIsShow){
                    this.yAxis.selectAll('.grid').attr('style', 'display: block');
                }else{
                    this.yAxis.selectAll('.grid').attr('style', 'display: none');
                }
            }
        },
        'options.tooltipIsShow' : {
            handler() {
                if(this.options.tooltipIsShow){
                   this.tooltip.attr('style','display: block');
                }else{
                    this.tooltip.attr('style','display: none');
                }
            }
        },
        'options.tooltipPadding': {
            handler() {
                this.tooltipPadding = this.options.tooltipPadding;
                let textWidth;
                let textHeight;
                const padding = this.tooltipPadding;
                const chartpadding = this.chartPadding;

                // 修改鼠标悬浮事件
                this.chart.on('mouseover', function (d) {
                    const x =d3.event.layerX-50-chartpadding.left;
                    const y =d3.event.layerY-chartpadding.top;
                    d3.select('.tooltip')
                        .attr('transform',`translate(${x},${y})`)
                        .attr('opacity',0.7);
                    d3.select('.tooltip').select('text')
                        .text(`${d.name}:${d.value}`)
                        .attr('width',function(){
                            textWidth = this.getComputedTextLength();
                            textHeight = this.getBBox().height;
                            return 14;
                        });
                    // 更新tooltip矩形的尺寸
                    d3.select('.tooltip').select('rect')
                        .attr('width',textWidth+padding.left+padding.right)
                        .attr('height',textHeight+padding.top+padding.bottom);
                });
                this.tooltip.select('text')
                    .attr('transform',`translate(${this.tooltipPadding.left},${this.tooltipPadding.top+12})`);
            }
        },
        'options.tooltipColor': {
            handler() {
                this.tooltip.select('rect')
                    .attr('fill',this.options.tooltipColor);
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
        'options.chartBackground': {
            handler(){
                this.svg.attr('style',`background: ${this.options.chartBackground}`);
            }
        },
        'options.chartPadding': {
            handler() {
                this.chartPadding = this.options.chartPadding;

                // 更新图表部分尺寸
                this.width = 700- this.chartPadding.right - this.chartPadding.left-180;
                this.height = 700 -this.chartPadding.bottom - this.chartPadding.top-80;

                // 更新比例尺
                this.xscale = d3.scaleBand().domain(this.nameArray).rangeRound([0,this.width]).padding(0.1);
                this.yscale = d3.scaleLinear().domain([0,d3.max(this.valueArray)*1.1]).rangeRound([this.height,0]);

                this.g.attr('transform',`translate(${this.chartPadding.left+40}, ${this.chartPadding.top+40})`);

                // 更新鼠标悬浮事件
                const chartpadding = this.chartPadding;
                this.chart.on('mouseover', function (d) {
                    const x =d3.event.layerX-50-chartpadding.left;
                    const y =d3.event.layerY-chartpadding.top;
                    d3.select('.tooltip')
                        .attr('transform',`translate(${x},${y})`)
                        .attr('opacity',0.7);
                    d3.select('.tooltip').select('text')
                        .text(`${d.name}:${d.value}`);
                });

                // 更新brush可刷选范围
                this.brush.extent([[0, 0], [this.width, this.height]]);
                this.gBrush.call(this.brush);
                
                // 更新图表的绘制
                this.updateGrid();
                this.updateBars();
                this.updateAxis();
                this.updateLegendPosition();
                this.updateTitle();
            }
        },
        'options.colorList': {
            handler() {
                for(let i = 0; i < this.data.length; i++){
                    this.data[i].color = this.options.colorList[i];
                }
                this.originData = this.data;
                // 更新颜色列表后重新传入新数据
                switch(this.options.sortType){
                    case 'null':  this.chart.data(this.data);
                                    break;
                    case 'ascending': this.chart.data(this.sortKeyAscending(this.originData));
                                        break;
                    case 'descending':  this.chart.data(this.sortKeyDescending(this.originData));
                                        break;
                    default: break;
                }

                // 更新bars和图例的颜色
                this.chart.select('rect')
                    .attr('fill',function (item) {
                    return item.color;
                });
                this.legend.select('rect')
                    .style('fill', function(d){
                        return d.color;
                    });
            }
        },
        'options.scrollingIsShow':{
            handler() {
                if(this.options.scrollingIsShow){
                    d3.select('#bar-chart-container').style('overflow','scroll');
                }else{
                    d3.select('#bar-chart-container').style('overflow','hidden');
                }
            }
        },
        'options.scrollingWidth':{
            handler(){
                if(this.options.scrollingIsShow){
                    d3.select('#bar-chart-container').style('width',`${this.options.scrollingWidth}px`);
                }
            }
        },
        'options.scrollingHeight':{
            handler(){
                if(this.options.scrollingIsShow){
                    d3.select('#bar-chart-container').style('height',`${this.options.scrollingHeight}px`);
                }
            }
        },
        'options.labelIsShow': {
            handler() {
                if(this.options.labelIsShow){
                    this.dataLabel.attr('style','display: block');
                }else{
                    this.dataLabel.attr('style','display: none');
                }
            }
        },
        'options.labelPosition': {
            handler() {
                if(this.options.labelIsShow){
                    this.updateDataLabel(); 
                }
            }
        },
        'options.sortType': {
            handler() {
                // 根据排序类型更新数据
                switch(this.options.sortType){
                    case 'null':    this.nameArray = this.data.map((item)=> { return item.name;});
                                    this.valueArray = this.data.map((item)=> { return item.value;});
                                    this.chart.data(this.data);
                                    break;
                    case 'ascending':   this.nameArray = this.ascendingData.map((item)=> { return item.name;});
                                        this.valueArray = this.ascendingData.map((item)=> { return item.value;});
                                        this.chart.data(this.ascendingData);
                                        break;
                    case 'descending':  this.nameArray = this.descendingData.map((item)=> { return item.name;});
                                        this.valueArray = this.descendingData.map((item)=> { return item.value;});
                                        this.chart.data(this.descendingData);
                                        break;
                    default: break;
                }

                // 更新排序后的x轴标签
                this.xscale = d3.scaleBand().domain(this.nameArray).rangeRound([0,this.width]).padding(0.1);
                
                // 更新图例颜色
                // this.legend.select('rect')
                //     .style('fill', function(d){
                //         return d.color;
                //     });
                
                this.updateYAxis();
                this.updateAxis();
                this.updateBars();
                
                
            }
        }

    },
    // https://cn.vuejs.org/v2/api/#methods
    methods: {
        // 这里是一些组件其余地方会使用到的函数，调用方式为 this.xxxx()
        sortKeyAscending(array,key){
            return array.sort(function(a,b){
                const x = a[key];
                const y = b[key];
                return ((y>x)?-1:(x<y)?1:0) ;
            });
        },
        sortKeyDescending(array,key){
            return array.sort(function(a,b){
                const x = a[key];
                const y = b[key];
                return ((y<x)?-1:(x>y)?1:0);   
            });
        },
        initBarChart() {
            console.log(this.options);

            // 获取x轴和y轴的标签
            this.nameArray = this.data.map((item)=> { return item.name;});
            this.valueArray = this.data.map((item)=> { return item.value;});

            // 指定图表的宽高
            this.width = 700 - this.chartPadding.right - this.chartPadding.left-180;
            this.height = 700 -this.chartPadding.bottom - this.chartPadding.top-80;

            // 比例尺
            const xscale = d3.scaleBand().domain(this.nameArray).rangeRound([0,this.width]).padding(0.1);
            const yscale = d3.scaleLinear().domain([0,d3.max(this.valueArray)*1.1]).rangeRound([this.height,0]).nice();
            this.xscale = xscale;
            this.yscale = yscale;

            d3.select('#bar-chart-container')
                .style('width','720px')
                .style('height','720px');
            
            // 添加svg
            this.svg = d3.select('#bar-chart-container').append('svg')
                            .attr('style','background: #eee')
                            .attr('width',700)
                            .attr('height',700);
            // 添加g标签 
            this.g = this.svg.append('g')
                        .attr('class','chart')  // 图表部分
                        .attr('transform',`translate(${this.chartPadding.left+40}, ${this.chartPadding.top+40})`);
            
            // 添加brush
            this.brush = d3.brushX()
                .extent([[0, 0], [this.width, this.height]]) // 刷选范围为图表部分
                .on('brush',this.brushed);
 
            this.gBrush = d3.select('.chart').append('g')
                .attr('class','brush')
                .call(this.brush);

            // 暂存几个变量,以便之后用到
            const chartpadding = this.chartPadding;
            const chartwidth = this.width;
            const chartheight = this.height;
            
            // 添加x轴
            this.xAxis=this.g.append('g')
                    .attr('class','axis')
                    .attr('transform',`translate(0, ${this.height})`)
                    .call(d3.axisBottom(this.xscale))
                    .attr('font-weight','blod')
                    .attr('font-size',13);
            this.xAxis.selectAll('.tick')
                    .select('line')
                    .attr('class', 'axis-tick');  // x轴刻度
            this.xAxis.selectAll('.tick')
                    .select('text')
                    .attr('class', 'axis-label');  // x轴标签

            this.xAxis.selectAll('.tick') 
                .each(function(){
                        d3.select(this).append('line')
                            .attr('class','grid')    // x轴网格线
                            .attr('style','display: none')
                            .attr('stroke', '#000')
                            .attr('stroke-dasharray', '10,10')
                            .attr('x1', 0)
                            .attr('y1', -chartheight)
                            .attr('x2', 0)
                            .attr('y2', 0);
                });
            
            // 添加y轴
            this.yAxis=this.g.append('g')
                    .attr('class','axis')
                    .call(d3.axisLeft(this.yscale).ticks(6));
            this.yAxis.selectAll('.tick')
                    .select('line')
                    .attr('class', 'axis-tick');  // y轴刻度
            this.yAxis.selectAll('.tick')
                    .select('text')
                    .attr('class', 'axis-label');   // y轴标签
            
            this.yAxis.selectAll('.tick')
                .each(function(){
                        d3.select(this).append('line')
                            .attr('class','grid')   // y轴网格线
                            .attr('stroke', '#000')
                            .attr('stroke-dasharray', '10,10')
                            .attr('x1', 0)
                            .attr('y1', 0)
                            .attr('x2', chartwidth)
                            .attr('y2', 0);
                });

            // 添加chart,实际上就是bars
            this.chart = this.g.selectAll('#bar-chart-container').data(this.data).enter().append('g');
            this.chart.append('rect')
                    .attr('class','bar')
                    .attr('stroke','black')
                    .attr('style','stroke-width:0')
                    .attr('z-index', '100')
                    .attr('x', function (item) {
                        return xscale(item.name);
                    })
                    .attr('width',this.xscale.bandwidth())
                    .attr('y', function (item) {
                        return yscale(item.value);
                    })
                    .attr('height', function (item) {
                        return chartheight - yscale(item.value);
                    })
                    .attr('fill',function (item) {
                        return item.color;
                    });
            
            // 添加数据标签
            this.dataLabel = this.chart.append('text')
                                    .attr('class','data-label')
                                    .attr('font-size',16)
                                    .attr('text-anchor','middle')
                                    .text(function (item) {
                                        return item.value;
                                    })
                                    .attr('x', function (item) {
                                        return xscale(item.name)+xscale.bandwidth()/2;
                                    })
                                    .attr('y', function (item) {
                                        return yscale(item.value)+16;
                                    });
                                    
            // 添加提示框
            this.tooltip = this.g.append('g')
                                .attr('class','tooltip') 
                                .attr('opacity',0); // 默认不显示

            this.tooltip.append('rect')
                        .attr('fill','#eeeeee')
                        .attr('rx',0)
                        .attr('ry',0)
                        .attr('stroke','black')
                        .attr('style','stroke-width:1');

            this.tooltip.append('text')
                        .attr('font-size',12)
                        .attr('transform',`translate(${this.tooltipPadding.left},${this.tooltipPadding.top+12})`);
                
            d3.select('.tooltip').select('rect')
                        .attr('width',75)
                        .attr('height',23);  // 为提示框设置默认尺寸
            
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

            // 添加图例
            this.legendBox = this.g.append('g')
                                .attr('style', 'display: block')
                                .attr('transform',`translate(${this.width+40},${this.height/2 - this.legendHeight/2})`);
            
            this.legend = this.legendBox.selectAll('.legend') 
                    .data(this.data)
                    .enter().append('g')
                    .attr('class', 'legend')
                    .attr('stroke','black')
                    .attr('style','stroke-width:0')
                    .attr('transform', function(d, i) { return (`translate(10, ${i * 25})`); });
            // 图例文本
            this.legend.append('text')
                .attr('transform','translate(50,30)')
                .style('text-anchor', 'middle') 
                .attr('font-size',14)
                .text(function(d) { 
                    return d.name;
                });
            // 图例矩形
            this.legend.append('rect')
                    .attr('transform','translate(5,15)')
                    .attr('width', 20)
                    .attr('height', 20) // 设低一些就是线，高一些就是面
                    .style('fill', function(d){
                        return d.color;
                    });

            // 用于取消点击图例后bar的边框效果
            this.chart.on('click', function () {
                    d3.selectAll('.bar').attr('opacity', 1)
                                        .attr('style', 'stroke-width:0');
                });
            
            // 图例点击后, 对应bar透明度增加并出现边框
            let name;
            this.legend.on('click', function (d) {
                    name = d.name;
                    d3.selectAll('.bar').attr('opacity', function(item){
                                                   if(item.name === name) return 0.7;
                                                   return 1; 
                                                });
                    
                    d3.selectAll('.bar').attr('style', function(item){
                                                   if(item.name === name) return 'stroke-width: 2';
                                                   return 'stroke-width:0'; 
                                                });
            });

            // bar的悬浮显示提示框
            this.chart.on('mouseover', function (d) {
                    const x =d3.event.layerX-50-chartpadding.left;
                    const y =d3.event.layerY-chartpadding.top;
                    d3.select('.tooltip')
                        .attr('transform',`translate(${x},${y})`)   // 提示框跟随鼠标移动
                        .attr('opacity',0.7);
                    d3.select('.tooltip').select('text')
                        .text(`${d.name}:${d.value}`);
                    
                })
                .on('mouseout', function () {
                    d3.select('.tooltip')
                        .attr('opacity',0);
                });
            
        },
        brushed() {
            const select= d3.event.selection;
            const xs = this.xscale;
            const d = xs.bandwidth()/2;
            // 刷选中的bar透明度增加并出现边框
            d3.selectAll('.bar')
                    .attr('opacity', function(item){
                                    const position =xs(item.name);
                                    if(position+d>=select[0]&& position+d<=select[1])
                                        return 0.8;
                                    return 1; 
                                })
                    .attr('style',function(item){
                            const position =xs(item.name);
                            if(position+d>=select[0]&& position+d<=select[1])
                                return 'stroke-width:2';
                            return 'stroke-width:0';
                        });

            // 刷选中的legend透明度增加并出现边框
            d3.selectAll('.legend')
                    .attr('style',function(item){
                            const position =xs(item.name);
                            if(position+d>=select[0]&& position+d<=select[1])
                                return 'stroke-width:1';
                            return 'stroke-width:0';
                        });
        },
        updateTitle() {
            if(this.options.titleIsShow){
                // 根据设置进行对应旋转和平移
                switch(this.options.titlePosition){
                    case 'top':     this.title.attr('transform','rotate(0) translate(0,0)');
                                    break;
                    case 'bottom':  this.title.attr('transform','rotate(0) translate(0,660)');
                                    break;
                    case 'left':    this.title.attr('transform','translate(0,700) rotate(270)');
                                    break;
                    case 'right':   this.title.attr('transform','translate(700,0) rotate(90)');
                                    break;
                    default: break;
                }
                    
            }
        },
        updateAxis(){
            let ys = this.yscale;
            // 更新x轴
            if(this.options.xAxisPosition==='top') {
                this.xAxis.attr('transform','translate(0, 0)').call(d3.axisTop(this.xscale));
                this.xAxis.selectAll('.tick').selectAll('.axis-label')
                                .attr('transform','translate(0,-5)');
                this.updateYAxis();
                ys = this.yscale;
                this.chart.selectAll('.bar').attr('y', 0);
            }else{
                this.xAxis.attr('transform',`translate(0, ${this.height})`).call(d3.axisBottom(this.xscale));
                this.xAxis.selectAll('.tick').selectAll('.axis-label')
                                .attr('transform','translate(0,0)');
                this.updateYAxis();
                ys = this.yscale;
                this.chart.selectAll('.bar').attr('y', function (item) {
                                                        return ys(item.value);
                                                    });
            }
            // 更新数据标签
            this.updateDataLabel();     
        },
        updateYAxis(){
            if(this.options.yAxisIsShow){
                    if(this.options.yAxisNice){ // 采用了nice
                        switch(this.options.yAxisScale){
                            case 'scaleLinear':  if(this.options.xAxisPosition==='bottom')
                                                    this.yscale = d3.scaleLinear().domain([0,d3.max(this.valueArray)*1.1]).rangeRound([this.height,0]).nice();
                                                else
                                                    this.yscale = d3.scaleLinear().domain([0,d3.max(this.valueArray)*1.1]).rangeRound([0, this.height]).nice();
                                                break;
                            case 'scaleBand':   this.yscale = d3.scaleBand().domain([0,d3.max(this.valueArray)*1.1]).rangeRound([0,this.height]).padding(0.1);
                                                break;
                            case 'scaleOrdinal': if(this.options.xAxisPosition==='bottom')
                                                    this.yscale = d3.scaleOrdinal().domain([0,d3.max(this.valueArray)*1.1]).nice();
                                                break;
                            case 'scaleQuantize': if(this.options.xAxisPosition==='bottom')
                                                    this.yscale = d3.scaleQuantize().domain([0,d3.max(this.valueArray)*1.1]).range([this.height,0]).nice();
                                                else   
                                                    this.yscale = d3.scaleQuantize().domain([0,d3.max(this.valueArray)*1.1]).range([0,this.height]).nice();
                                                break;
                            case 'scaleTime': if(this.options.xAxisPosition==='bottom')
                                                this.yscale = d3.scaleTime().domain([0,d3.max(this.valueArray)*1.1]).rangeRound([this.height,0]).nice();
                                            else
                                                this.yscale = d3.scaleTime().domain([0,d3.max(this.valueArray)*1.1]).rangeRound([0,this.height]).nice();
                                                break;
                            default: break;

                        }
                    }else{  // 未采用nice
                        switch(this.options.yAxisScale){
                            case 'scaleLinear':  if(this.options.xAxisPosition==='bottom')
                                                    this.yscale = d3.scaleLinear().domain([0,d3.max(this.valueArray)*1.1]).rangeRound([this.height,0]);
                                                else
                                                    this.yscale = d3.scaleLinear().domain([0,d3.max(this.valueArray)*1.1]).rangeRound([0, this.height]);
                                                break;
                            case 'scaleBand':   this.yscale = d3.scaleBand().domain([0,d3.max(this.valueArray)*1.1]).rangeRound([0,this.height]).padding(0.1);
                                                break;
                            case 'scaleOrdinal': if(this.options.xAxisPosition==='bottom')
                                                    this.yscale = d3.scaleOrdinal().domain([0,d3.max(this.valueArray)*1.1]);
                                                break;
                            case 'scaleQuantize': if(this.options.xAxisPosition==='bottom')
                                                    this.yscale = d3.scaleQuantize().domain([0,d3.max(this.valueArray)*1.1]).range([this.height,0]);
                                                else   
                                                    this.yscale = d3.scaleQuantize().domain([0,d3.max(this.valueArray)*1.1]).range([0,this.height]);
                                                break;
                            case 'scaleTime': if(this.options.xAxisPosition==='bottom')
                                                this.yscale = d3.scaleTime().domain([0,d3.max(this.valueArray)*1.1]).rangeRound([this.height,0]);
                                            else
                                                this.yscale = d3.scaleTime().domain([0,d3.max(this.valueArray)*1.1]).rangeRound([0,this.height]);
                                                break;
                            default: break;

                        }
                    }
                    if(this.options.yAxisPosition==='left')
                        this.yAxis.attr('transform','translate(0, 0)').call(d3.axisLeft(this.yscale).ticks(6));
                    else
                        this.yAxis.attr('transform',`translate(${this.width},0)`).call(d3.axisRight(this.yscale).ticks(6));
                }
        },
        updateBars(){
            // 重新绘制bars
            const chartheight = this.height;
            const xs = this.xscale;
            const ys = this.yscale;

            this.chart.selectAll('rect')
                    .attr('class','bar')
                    .attr('x', function (item) {
                        return xs(item.name);
                    })
                    .attr('width',this.xscale.bandwidth())
                    .attr('y', function (item) {
                        return ys(item.value);
                    })
                    .attr('height', function (item) {
                        return chartheight - ys(item.value);
                    })
                    .attr('fill',function (item) {
                        return item.color;
                    });
        },
        updateGrid(){
            if(this.options.yAxisPosition==='left') {
                this.yAxis.selectAll('.grid').attr('x2', this.width);
            }else{
                this.yAxis.selectAll('.grid').attr('x2', -this.width);
            }
            if(this.options.xAxisPosition==='bottom') {
                this.xAxis.selectAll('.grid').attr('y1', -this.height);
            }else{
                this.xAxis.selectAll('.grid').attr('y1', this.height);
            }
        },
        updateDataLabel(){
            const ys = this.yscale;
            const h = this.height;
            if(this.options.xAxisPosition==='top') {
                switch(this.options.labelPosition){
                    case 'top': this.dataLabel.attr('y', function (item) {
                                                        return ys(item.value)-5;
                                                        });
                                break;
                    case 'bottom':  this.dataLabel.attr('y',16);
                                    break;
                    case 'innner':  this.dataLabel.attr('y', function (item) {
                                                                return ys(item.value)/2;
                                                            });
                                    break;
                    default: break;
                }
            }else{
                switch(this.options.labelPosition){
                    case 'top': this.dataLabel.attr('y', function (item) {
                                                            return ys(item.value)+16;
                                                        });
                                break;
                    case 'bottom':  this.dataLabel.attr('y',this.height-5);
                                    break;
                    case 'innner':  this.dataLabel.attr('y', function (item) {
                                                                return (h - ys(item.value))/2+ys(item.value);
                                                            });
                                    break;
                    default: break;
                }
            }
            const xs = this.xscale;
            this.dataLabel.attr('x', function (item) {
                        return xs(item.name)+xs.bandwidth()/2;
                    });
        },
        updateLegendPosition(){
            switch(this.options.legendPosition){
                case 'top': this.legendBox.attr('transform',`translate(${this.width/2-this.legendWidth/2},${ -this.legendHeight-20})`);
                            break;
                case 'bottom': this.legendBox.attr('transform',`translate(${this.width/2-this.legendWidth/2},${this.height+20})`);
                            break;
                case 'left': this.legendBox.attr('transform',`translate(${-this.legendWidth-40},${this.height/2 - this.legendHeight/2})`);
                            break;
                case 'right': this.legendBox.attr('transform',`translate(${this.width+40},${this.height/2 - this.legendHeight/2})`);
                                break;
                case 'left-top': this.legendBox.attr('transform',`translate(${-this.legendWidth-40},0)`);
                                    break;
                case 'right-top': this.legendBox.attr('transform',`translate(${this.width+40},0)`);
                                    break;
                case 'left-bottom': this.legendBox.attr('transform',`translate(${-this.legendWidth-40},${this.height - this.legendHeight})`);
                                    break;
                case 'right-bottom': this.legendBox.attr('transform',`translate(${this.width+40},${this.height - this.legendHeight})`);
                                    break;
                default: break;
            }
        }
    }

};
</script>
<style scoped>
text {
    background: '#000';
}
#bar-chart-container {
    overflow: hidden;
}
</style>