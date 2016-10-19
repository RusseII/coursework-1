Cjo= 59.4*(10^-12);
M= 2.3;
Cp= 2*(10^-12);
Vj= 6.5;
L= 1.83*(10^-3);
V1=0.5;
V2=2.5;

Cv1=(((Cjo)/((1+((V1)/Vj))^M))+Cp)


Cv2=(((Cjo)/((1+((V2)/Vj))^M))+Cp);

Rat = Cv1/Cv2

Lmin = (1.1)/(((2*pi*540000)^2)*Cv1)

Cmin = (1/((((2*pi*540000)^2)*Lmin)-(1/Cv1)))
 
C = (1/((((2*pi*540000)^2)*Lmin)-(1/Cv1)))