# 42_miniRT_wip

### Ray equation

$ray(t) = o + d * t$  

- t: scalar parameter  
- d: direction (normalized)  
- o: origin point (camera)  


### Plane equation

The vector formed from any two points of a plane and its normal vector are perpendicular.  

$(x - p) · n = 0$  

- x: any point on the plane  
- p: known point on the plane  
- n: normal vector  

### Intersection of a ray with a plane

Let's substitute the ray in the plane equation and solve for t.  

$((o + d * t) - p) · n = 0$  
$(d * t + o - p) · n = 0$  
$(d * t) · n + (o - p) · n = 0$  
$(d * t) · n = (p - o) · n$  
$t = \frac{(p - o) · n}{d · n}$  

Edge cases:  
- The denominator d · n == 0, the ray and the plane are parallel.  
  - If the numerator (p - o) · n != 0, the ray is outside the plane, there is no intersection.  
  - If the numerator (p - o) · n == 0, the ray is in the plane, the are infinite intersections.  
- t < 0, the intersection is behind the camera, it is not considered.  
- t >= 0, t indicates the distance from the origin of the ray to the intersection.  

### Sphere equation

Every point on the surface of the sphere are at a fixed distance from the center.  

$|x - c| = r$  

- x: any point on the sphere  
- c: center of the sphere  
- r: radius of the sphere  

### Intersection of a ray with a sphere

Let's substitute the ray in the sphere equation and solve for t.  
We will not solve using the magnitude formula because it has a square root.  
We will substitute k = o - c for clarity.  
We will use the fact that the |d·d| == 1, since d is normalized.  

$|(o + d * t) - c| = r$  
$|d * t + o - c| = r$  
$|d * t + k| = r$  
$(d * t + k) · (d * t + k) = r²$  
$(d · d) t²+ 2(k · d)t + k · k = r²$  
$t²+ 2(k · d)t + (k · k - r²) = 0$  
$t = \frac{-2d · k \pm \sqrt{4(d · k)²-4(k · k - r²)}}{2}$  
$t = \frac{-2d · k \pm 2 \sqrt{(d · k)²-k · k + r²}}{2}$  
$t = -d · k \pm \sqrt{(d · k)²-k · k + r²}$  

Edge cases:  
- The discriminant < 0, there's no solutions so the ray doesn't intersect the sphere.  
- The discriminant == 0, there's one solutions so the ray is tangent to the sphere.  
  - If t < 0, the intersection is behind the camera, it is not considered.  
  - If t >= 0, t indicates the distance from the origin of the ray to the intersection.  
- The discriminant > 0, there's two solutions so the ray intersects the sphere twice.  
  - If t_1 < t_2 < 0, the sphere is behind the camera, it is not considered.  
  - If 0 < t_1 < t_2, t_1 indicates the distance from the origin of the ray to the intersection.  
  - If t_1 < 0 < t_2, the origin of the ray is inside the sphere.  
