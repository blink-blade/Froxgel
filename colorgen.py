n = 500
half = n // 2

colors = []

# blue -> green
for i in range(half):
    t = i/(half-1)
    r = 0
    g = t
    b = 1-t
    colors.append(f"vec3({r:.3f},{g:.3f},{b:.3f})")

# green -> red
for i in range(n-half):
    t = i/(n-half-1)
    r = t
    g = 1-t
    b = 0
    colors.append(f"vec3({r:.3f},{g:.3f},{b:.3f})")

print("vec3 colors[500]=vec3[](" + ",".join(colors) + ");")