#version 410
// geometry shader

layout (lines_adjacency) in;
layout (line_strip, max_vertices = 10) out;

// Outputs separate colors for the line and the curvature comb
out vec4 line_color;

// calculates the curvature at b in line segment a-b-c
float calcCurvature(vec4 a, vec4 b, vec4 c) {
	float dotProduct = dot(c - b, a - b);
	float angle = acos(dotProduct / (length(c - b) * length(a - b)));
	float curvature = (2 * sin(angle) / length(c - a));
	return curvature;
}

// Calculates the normal at b in line segment a-b-c
vec2 calcNormal(vec4 a, vec4 b, vec4 c) {
	vec2 normal = normalize(b.xy - (b.xy + normalize(c.xy - b.xy) + normalize(a.xy - b.xy)));
	return normal;
}

// Emits a line from pointA to pointB
void emitLine(vec4 pointA, vec4 pointB) {
	gl_Position = pointA;
	EmitVertex();
	gl_Position = pointB;
	EmitVertex();
	EndPrimitive();
}

// Constructs the comb teeth and connecting lines from the curve points. While drawing two teeth for each line
// means that each tooth will be drawn twice, it does make sure that we always have both the first and the last
// tooth drawn, and that all connecting lines go between two teeth instead of either the first or the last one
// going between a tooth and the place where a tooth would have been.
void main() {

  // Determines the magnitude of the visualisation
  float curvatureVisualisationSize = 0.05;


  // Emit the curve itself
  line_color = vec4(0.8, 0.8, 0.8, 1.0);
  emitLine(gl_in[1].gl_Position, gl_in[2].gl_Position);


  // calculate curvature for the first tooth
  float curvature = calcCurvature(gl_in[0].gl_Position, gl_in[1].gl_Position, gl_in[2].gl_Position);
  // derive normal for the first tooth
  vec2 normal = calcNormal(gl_in[0].gl_Position, gl_in[1].gl_Position, gl_in[2].gl_Position);
  // emit the first comb tooth
  line_color = vec4(0.3, 0.3, 0.3, 1.0);
  vec4 firstToothTip = gl_in[1].gl_Position;
  firstToothTip.xy += normal * curvature * curvatureVisualisationSize;
  emitLine(gl_in[1].gl_Position, firstToothTip);


  // calculate curvature for the second tooth
  curvature = calcCurvature(gl_in[1].gl_Position, gl_in[2].gl_Position, gl_in[3].gl_Position);
  // derive normal for the second tooth
  normal = calcNormal(gl_in[1].gl_Position, gl_in[2].gl_Position, gl_in[3].gl_Position);
  // emit the second comb tooth
  vec4 secondToothTip = gl_in[2].gl_Position;
  secondToothTip.xy += normal * curvature * curvatureVisualisationSize;
  emitLine(gl_in[2].gl_Position, secondToothTip);


  // emit the connecting line
  emitLine(firstToothTip, secondToothTip);
}
