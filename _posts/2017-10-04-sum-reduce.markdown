---
layout: post
comments: true
title:  "Sum Reduce Implementation in OpenCL"
excerpt: "I can't believe I lasted this long on Wordpress. I am switching permanently to Jekyll for hosting my blog, and so should you :) Details inside."
date:   2014-07-01 20:00:00
---

*"So what's wrong with Wordpress?"* You may ask. Let's see, everything:


> Wordpress is a bloated, clunky, slow, vulnerable, closed mess.

#### Example workflow
To give a flavor for the workflow, to add a new blog post I proceed as follows:

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

