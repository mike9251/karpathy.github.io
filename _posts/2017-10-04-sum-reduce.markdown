---
layout: post
comments: true
title:  "Sum Reduce Implementation in OpenCL"
excerpt: "I can't believe I lasted this long on Wordpress. I am switching permanently to Jekyll for hosting my blog, and so should you :) Details inside."
date:   2017-10-04 20:00:00
---

Reduce operation combines elements of an array into a single value.
It has two input parameters:  
1. Set of elements
2. Reduction operator (+, ||, &, min, max).  

Reduction operator should be binary (perform operation on two values) and associative (a op b op c == c op b op a). 
Parallel implementation can be viewed as followed:
a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10.
Each work-item in the first half of WorkGroup re-calculate their values as value[id] = value[id] + value[id + offset]
where offset = num_elements/2. Each iteration reduces the number of values by 2.
Parallel reduce has step step complexity logn and work complexity n. 

```C
__kernel void sum_reduce_kernel(__global int *input, __global int * output, __local int *sdata)
{
    int idx = get_global_id(0);
    int lidx = get_local_id(0);
    int groupSize = get_local_size(0);
    int groupId = get_group_id(0);

    sdata[lidx] = input[idx];
    barrier(CLK_LOCAL_MEM_FENCE);

    for(int i = groupSize / 2; i > 0; i >>= 1)
    {
        if(lidx < i)
        {
            sdata[lidx] += sdata[lidx + i];
        }
        barrier(CLK_LOCAL_MEM_FENCE);
    }
    if(lidx == 0)
    {
        output[groupId] = sdata[lidx];
    }
}
```

Anyway, that's just a brief taste. Check out [Jekyll](http://jekyllrb.com/) and get blogging in a sane way!

