/*
 * rg_engine / vecmath.js
 * (c) Alex9932 19.11.2021
 */

//
// Functions description
//
// addS Add a - vector, b - number, return - vector
// subS Sub a - vector, b - number, return - vector
// mulS Mul a - vector, b - number, return - vector
// add  Add a, b - vectors, return - vector
// sub  Sub a, b - vectors, return - vector
// mul  Mul a, b - vectors, return - vector
// dot  Dot product a, b - vectors, return - number
// len  Length a, b - vectors, return - number
// norm Normalize a, b - vectors, return - vector
//

function vec2(x, y) {
    return {
        x: x,
        y: y,
        to_string: function() {
            return "Vec2: " + this.x + " " + this.y;
        }
    };
}

function vec3(x, y, z) {
    return {
        x: x,
        y: y,
        z: z,
        to_string: function() {
            return "Vec3: " + this.x + " " + this.y + " " + this.z;
        }
    };
}

function vec4(x, y, z, w) {
    return {
        x: x,
        y: y,
        z: z,
        w: w,
        to_string: function() {
            return "Vec4: " + this.x + " " + this.y + " " + this.z + " " + this.w;
        }
    };
}

// Vec2
function vec2_addS(a, b) { return vec2(a.x + b, a.y + b); }
function vec2_subS(a, b) { return vec2(a.x - b, a.y - b); }
function vec2_mulS(a, b) { return vec2(a.x * b, a.y * b); }
function vec2_add (a, b) { return vec2(a.x + b.x, a.y + b.y); }
function vec2_sub (a, b) { return vec2(a.x - b.x, a.y - b.y); }
function vec2_mul (a, b) { return vec2(a.x * b.x, a.y * b.y); }
function vec2_dot (a, b) { return a.x * b.x + a.y * b.y; }
function vec2_len (a)    { return Math.sqrt(a.x * a.x + a.y * a.y); }
function vec2_norm(a) {
    var len = vec2_len(a);
    return vec2(a.x / len, a.y / len);
}

// Vec3
function vec3_addS(a, b) { return vec3(a.x + b, a.y + b, a.z + b); }
function vec3_subS(a, b) { return vec3(a.x - b, a.y - b, a.z - b); }
function vec3_mulS(a, b) { return vec3(a.x * b, a.y * b, a.z * b); }
function vec3_add (a, b) { return vec3(a.x + b.x, a.y + b.y, a.z + b.z); }
function vec3_sub (a, b) { return vec3(a.x - b.x, a.y - b.y, a.z - b.z); }
function vec3_mul (a, b) { return vec3(a.x * b.x, a.y * b.y, a.z * b.z); }
function vec3_dot (a, b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
function vec3_len (a)    { return Math.sqrt(a.x * a.x + a.y * a.y + a.z * a.z); }
function vec3_norm(a) {
    var len = vec3_len(a);
    return vec3(a.x / len, a.y / len, a.z / len);
}

// Vec4
function vec4_addS(a, b) { return vec4(a.x + b, a.y + b, a.z + b, a.w + b); }
function vec4_subS(a, b) { return vec4(a.x - b, a.y - b, a.z - b, a.w - b); }
function vec4_mulS(a, b) { return vec4(a.x * b, a.y * b, a.z * b, a.w * b); }
function vec4_add (a, b) { return vec4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w); }
function vec4_sub (a, b) { return vec4(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w); }
function vec4_mul (a, b) { return vec4(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w); }
function vec4_dot (a, b) { return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w; }
function vec4_len (a)    { return Math.sqrt(a.x * a.x + a.y * a.y + a.z * a.z + a.w * a.w); }
function vec4_norm(a) {
    var len = vec4_len(a);
    return vec4(a.x / len, a.y / len, a.z / len, a.w / len);
}
