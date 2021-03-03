// 这里记录的是图表各种默认配置，当用户在测试页面选择不同的选项时，也就是是选择了这里的某组配置参数。
export default {
    leafNodeFill:{
        type: 'color',
        title: '叶子节点填充色',
        default: '#80C67D'
    },
    nonLeafNodeFill:{
        type: 'color',
        title: '非叶子节点填充色',
        default: '#FFFFFF'
    },
    nodeSize: {
        type: 'number',
        title: '节点大小',
        default: 5,
        values: [1, 12],
    },
    nodeBorderWidth: {
        type: 'number',
        title: '节点边框宽度',
        default: 2,
        values: [1, 10],
    },
};