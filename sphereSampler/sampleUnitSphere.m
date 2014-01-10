
numpoints = 500;

% Uniformly distribute 162 particles across the surface of the unit sphere 
[V,Tri,~,Ue]=ParticleSampleSphere('N',numpoints); % this operation takes ~8 sec on my machine (6GB RAM & 2.8 GHz processor)

% For each point, compute normal and uv
dims = 3 + 4 + 3 + 3 + 3; %pos[3], color[4], uvec[3], vvec[3], normal[3]
S = zeros(numpoints,dims);

c = [1.0 0.0 0.0 1.0]; %full red

for i = 1:1:numpoints
    p = V(i,:);
    
    n = 2 * p; %normal
    n = n ./ norm(n);
    
    u = [1 1 (p(1) + p(2)) / p(3)];
    v = cross(u,n);
    
    u = u ./ norm(u);
    v = v ./ norm(v);
    
    S(i,:) = [p c u v n];
end

dlmwrite(['spherePoints' num2str(numpoints) '.txt'],S,'delimiter',' ', 'precision','%1.3f');

% Visualize the mesh based on the optimal configuration of particles 
TR=TriRep(Tri,V); 
figure, h=trimesh(TR); set(h,'EdgeColor','b'), axis equal