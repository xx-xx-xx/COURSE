// 这里记录的是图表各种默认配置，当用户在测试页面选择不同的选项时，也就是是选择了这里的某组配置参数。
export default {
    linkStrokeColor:{
        type: 'color',
        title: '线体颜色',
        default: '#000'
    },
    linkStrokeOpacity:{
        type: 'number',
        title: '不透明度',
        default: 6,
        values: [0, 10]
    },
    linkStrokeWidth:{
        type: 'number',
        title: '线体宽度',
        default: 2,
        values: [1,5]
    }
};