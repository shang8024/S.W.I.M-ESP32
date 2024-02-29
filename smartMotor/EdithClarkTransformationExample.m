load EdithClarkeTransformationExampleData.csv
t=EdithClarkeTransformationExampleData(:,1);
a=EdithClarkeTransformationExampleData(:,2);
b=EdithClarkeTransformationExampleData(:,3);
c=EdithClarkeTransformationExampleData(:,4);
D=EdithClarkeTransformationExampleData(:,2:4); % ABC data

s=sqrt(3)/2; E=[1 -.5 -.5; 0 s -s; .5 .5 .5] % Edith Clarke matrix
E=E*sqrt(2/3); % make the matrix unitary

ED=(E*D')';

subplot(311)
plot(t,a,t,b,t,c)
title("3 phase data from ODrive")
subplot(312)
plot(t,ED(:,1),t,ED(:,2),t,ED(:,3))
title("Edith Clarke transformation of data")

Einv=inv(E);
D=(Einv*ED')';
subplot(313)
plot(t,D(:,1),t,D(:,2),t,D(:,3))
title("Inverse Edith Clarke transformation of transformed data")
xlabel("(Should be same as original data)")

print EdithClarkTransformationExample.pdf; 