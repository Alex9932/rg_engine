include("gamedata/scripts/vecmath.js");

var a = vec2_norm(vec2(1, 1));
var b = vec2_norm(vec2(1, 1));

var c = vec2_dot(a, b);

print(a.to_string());
print(b.to_string());
print("Dot: " + c);

print("A's length: " + vec2_len(a));
