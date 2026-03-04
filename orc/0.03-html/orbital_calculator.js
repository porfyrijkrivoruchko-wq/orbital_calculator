var Gamma=6.6743e-11;

function SkyBody(n,m,r,p,rn) {
  this.name=n;
  this.mass=m;
  this.radius=r;
  this.period=p;
  this.runame=rn;
  this.MG=m*Gamma;
}

var bodies=[
  new SkyBody('sun',1.9885e30,6.96e8,25.38*86400,'Солнце'),
  new SkyBody('mercury',3.33022e23,2.4397e6,58.646*86400,"Меркурий"),
  new SkyBody('venus',4.8675e24,6.0518e6,243.023*86400,"Венера"),
  new SkyBody('earth',5.9726e24,6.371e6,0.99726968*86400,"Земля"),
  new SkyBody('moon',7.3477e22,1.7371e6,27.321661*86400,"Луна"),
  new SkyBody('mars',6.4171e23,3.3895e6,24.6229*3600,"Марс"),
  new SkyBody('yupiter',1.8986e27,69.911e6,9.925*3600,"Юпитер"),
  new SkyBody('saturn',5.6846e26,58.232e6,10*3600+32*60+45,"Сатурн"),
  new SkyBody('uran',8.6813e25,25.362e6,0.71833*86400,"Уран"),
  new SkyBody('neptun',1.02409e26,24.622e6,0.6653*86400,"Нептун"),
  new SkyBody('pluto',1.303e22,1.1883e6,6.387*86400,"Плутон"),
  new SkyBody('cerera',9.393e20,463.5e3,9*3600+4*60+27.01,"Церера"),
  new SkyBody('vesta',2.59076e20,525.4e3/2,5.342*3600,"Веста"),
  new SkyBody('erida',1.67e22,1.163e6,25.9*3600,"Эрида"),
  new SkyBody('ganimed',1.4819e23,2.6341e6,7.15455296*86400,"Ганимед"),
  new SkyBody('europe',4.817e22,1,5608e6,3,551*86400,"Европа"),
  new SkyBody('titan',1.3e23,5.15e6/2,15.25*86400,"Титан"),
  new SkyBody('encelad',1.08022e20,252.1e3,118386.82,"Энцелад")
], orbitType= ['circle','ellipse','parabolic','hyperbolic'];

function sqr(x) { return x*x; }

function cube(x) { return x*x*x; }

var sunit=['munit','runit','punit'];
var orbit,point;

function setRadiusCircle(r) {
  this.radius=r;
  this.speed=Math.sqrt(this.center.MG/this.radius);
  this.period=2*Math.PI*this.radius/this.speed;
}

function setPeriodCircle(p) {
  this.period=p;
  this.speed=Math.pow(2*Math.PI*this.center.MG/this.period,1./3);
  this.radius=this.speed*this.period/(2*Math.PI);
}

function setSpeedCircle(s) {
  this.speed=s;
  this.radius=this.center.MG/sqr(this.speed);
  this.period=2*Math.PI*this.center.MG/cube(this.speed);
}

function cleanCircle() {
  this.radius=null;
  this.speed=null;
  this.period=null;
}

function Circle2Circle() {
  return this;
}

function Circle2Ellipse() {
  var e=new EllipseOrbit(this.center);
  if(this.radius) {
    e.setPerigey(this.radius);
    e.setApogey(this.radius);
  }
  return e;
}

function Circle2Parabolic() {
  var p=new ParabolicOrbit(this.center);
  if(this.radius)
    p.setPerigey(this.radius);
}

function Circle2Hyperbolic() {
  var h=new HyperbolicOrbit(this.center);
  if(this.radius)
    p.setPerigey=this.radius;
  return h;
}

function CheckCircle() {
  if(this.radius && this.radius<this.center.radius)
    return false;
  if(this.speed && this.speed<=0)
    return false;
  if(this.period && this.period<=0)
    return false;
  return true;
}

function CircleOrbit(b) {
  this.center=b;
  this.setRadius=setRadiusCircle;
  this.setPeriod=setPeriodCircle;
  this.setSpeed=setSpeedCircle;
  this.clean=cleanCircle;
  this.circle=Circle2Circle;
  this.ellipse=Circle2Ellipse;
  this.parabolic=Circle2Parabolic;
  this.hyperbolic=Circle2Hyperbolic;
  this.check=CheckCircle;
}

function setPerigeyEllipse(p) {
  this.perigey=p;
  if(this.apogey) {
    this.a=(this.perigey+this.apogey)/2;
    this.period=2*Math.PI*this.a*Math.sqrt(this.a/this.center.MG);
    this.excentrisitet=(this.apogey-this.perigey)/(this.apogey+this.perigey);
    this.b=this.a*Math.sqrt(1-sqr(this.excentrisitet));
    this.pspeed=Math.sqrt(orbit.center.MG*orbit.apogey/orbit.a/orbit.perigey);
    this.aspeed=Math.sqrt(orbit.center.MG*orbit.perigey/orbit.a/orbit.apogey);
  } else if(this.a) {
    this.apogey=this.a*2-this.perigey;
    this.excentrisitet=(this.a-this.perigey)/this.a;
    this.b=this.a*Math.sqrt(1-sqr(this.excentrisitet));
    this.pspeed=Math.sqrt(orbit.center.MG*orbit.apogey/orbit.a/orbit.perigey);
    this.aspeed=Math.sqrt(orbit.center.MG*orbit.perigey/orbit.a/orbit.apogey);
  } else if(this.excentrisitet) {
    this.apogey=this.perigey*(1+this.excentrisitet)/(1-this.excentrisitet);
    this.a=(this.perigey+this.apogey)/2;
    this.period=2*Math.PI*this.a*Math.sqrt(this.a/this.center.MG);
    this.b=this.a*Math.sqrt(1-sqr(this.excentrisitet));
    this.pspeed=Math.sqrt(orbit.center.MG*orbit.apogey/orbit.a/orbit.perigey);
    this.aspeed=Math.sqrt(orbit.center.MG*orbit.perigey/orbit.a/orbit.apogey);
  } else if(this.b) {
    this.apogey=sqr(this.b)/this.perigey;
    this.a=(this.apogey+this.perigey)/2;
    this.excentrisitet=(this.a-this.perigey)/this.a;
    this.period=2*Math.PI*this.a*Math.sqrt(this.a/this.center.MG);
    this.pspeed=Math.sqrt(orbit.center.MG*orbit.apogey/orbit.a/orbit.perigey);
    this.aspeed=Math.sqrt(orbit.center.MG*orbit.perigey/orbit.a/orbit.apogey);
  } else if(this.pspeed) {
    this.apogey=sqr(this.perigey)/(2*this.center.MG/sqr(this.pspeed)-this.perigey);
    this.a=(sqr(this.b)/this.perigey+this.perigey)/2;
    this.excentrisitet=(this.a-this.perigey)/this.a;
    this.b=this.a*Math.sqrt(1-sqr(this.excentrisitet));
    this.period=2*Math.PI*this.a*Math.sqrt(this.a/this.center.MG);
    this.aspeed=this.pspeed*this.perigey/this.apogey;
  } else if(this.aspeed) {
    this.apogey=Math.sqrt(sqr(this.perigey)/2+2*this.center.MG*this.perigey/sqr(this.aspeed))-this.perigey/2;
    this.a=(this.perigey+this.apogey)/2;
    this.excentrisitet=(this.a-this.perigey)/this.a;
    this.b=this.a*Math.sqrt(1-sqr(this.excentrisitet));
    this.period=2*Math.PI*this.a*Math.sqrt(this.a/this.center.MG);
    this.pspeed=this.aspeed*this.apogey/this.perigey;
  }
}

function setApogeyEllipse(a) {
  this.apogey=a;
  if(this.perigey) {
    this.a=(this.perigey+this.apogey)/2;
    this.period=2*Math.PI*this.a*Math.sqrt(this.a/this.center.MG);
    this.excentrisitet=(this.apogey-this.perigey)/(this.apogey+this.perigey);
    this.b=this.a*Math.sqrt(1-sqr(this.excentrisitet));
    this.pspeed=Math.sqrt(orbit.center.MG*orbit.apogey/orbit.a/orbit.perigey);
    this.aspeed=Math.sqrt(orbit.center.MG*orbit.perigey/orbit.a/orbit.apogey);
  } else if(this.a) {
    this.perigey=2*this.a-this.apogey;
    this.excentrisitet=(this.a-this.perigey)/this.a;
    this.b=this.a*Math.sqrt(1-sqr(this.excentrisitet));
    this.pspeed=Math.sqrt(orbit.center.MG*orbit.apogey/orbit.a/orbit.perigey);
    this.aspeed=Math.sqrt(orbit.center.MG*orbit.perigey/orbit.a/orbit.apogey);
  } else if(this.b) {
    this.perigey=sqr(b)/this.apogey;
    this.a=(this.perigey+this.apogey)/2;
    this.excentrisitet=(this.a-this.perigey)/this.a;
    this.period=2*Math.PI*this.a*Math.sqrt(this.a/this.center.MG);
    this.pspeed=Math.sqrt(orbit.center.MG*orbit.apogey/orbit.a/orbit.perigey);
    this.aspeed=Math.sqrt(orbit.center.MG*orbit.perigey/orbit.a/orbit.apogey);
  } else if(this.excentrisitet) {
    this.perigey=this.apogey*(1-this.excentrisitet)/(1+this.excentrisitet);
    this.a=(this.perigey+this.apogey)/2;
    this.period=2*Math.PI*this.a*Math.sqrt(this.a/this.center.MG);
    this.b=this.a*Math.sqrt(1-sqr(this.excentrisitet));
    this.pspeed=Math.sqrt(orbit.center.MG*orbit.apogey/orbit.a/orbit.perigey);
    this.aspeed=Math.sqrt(orbit.center.MG*orbit.perigey/orbit.a/orbit.apogey);
  } else if(this.aspeed) {
    this.perigey=sqr(this.apogey)/(2*this.center.MG/sqr(this.aspeed)-this.apogey);
    this.a=(this.perigey+this.apogey)/2;
    this.excentrisitet=(this.a-this.perigey)/this.a;
    this.period=2*Math.PI*this.a*Math.sqrt(this.a/this.center.MG);
    this.b=this.a*Math.sqrt(1-sqr(this.excentrisitet));
    this.pspeed=this.aspeed*this.apogey/this.perigey;
  } else if(this.pspeed) {
    this.perigey=Math.sqrt(sqr(this.apogey)/2+2*this.center.MG*this.apogey/sqr(this.pspeed))-this.apogey/2;
    this.a=(this.perigey+this.apogey)/2;
    this.excentrisitet=(this.a-this.perigey)/this.a;
    this.period=2*Math.PI*this.a*Math.sqrt(this.a/this.center.MG);
    this.b=this.a*Math.sqrt(1-sqr(this.excentrisitet));
    this.aspeed=this.pspeed*this.perigey/this.apogey;
  }
}

function setBigSizeEllise(a) {
  this.a=a;
  this.period=2*Math.PI*this.a*Math.sqrt(this.a/this.center.MG);
  if(this.b) {
    this.perigey=this.a-Math.sqrt(sqr(this.a)-sqr(this.b));
    this.apogey=2*this.a-this.perigey;
    this.excentrisitet=(this.apogey-this.perigey)/(this.apogey+this.perigey);
    this.pspeed=Math.sqrt(orbit.center.MG*orbit.apogey/orbit.a/orbit.perigey);
    this.aspeed=Math.sqrt(orbit.center.MG*orbit.perigey/orbit.a/orbit.apogey);
  } else if(this.perigey) {
    this.apogey=this.a*2-this.perigey;
    this.excentrisitet=(this.a-this.perigey)/this.a;
    this.b=this.a*Math.sqrt(1-sqr(this.excentrisitet));
    this.pspeed=Math.sqrt(orbit.center.MG*orbit.apogey/orbit.a/orbit.perigey);
    this.aspeed=Math.sqrt(orbit.center.MG*orbit.perigey/orbit.a/orbit.apogey);
  } else if(this.apogey) {
    this.perigey=2*this.a-this.apogey;
    this.excentrisitet=(this.a-this.perigey)/this.a;
    this.b=this.a*Math.sqrt(1-sqr(this.excentrisitet));
    this.pspeed=Math.sqrt(orbit.center.MG*orbit.apogey/orbit.a/orbit.perigey);
    this.aspeed=Math.sqrt(orbit.center.MG*orbit.perigey/orbit.a/orbit.apogey);
  } else if(this.excentrisitet) {
    this.perigey=this.a*(1-this.excentrisitet);
    this.apogey=this.a*(1+this.excentrisitet);
    this.b=this.a*Math.sqrt(1-sqr(this.excentrisitet));
    this.pspeed=Math.sqrt(orbit.center.MG*orbit.apogey/orbit.a/orbit.perigey);
    this.aspeed=Math.sqrt(orbit.center.MG*orbit.perigey/orbit.a/orbit.apogey);
  } else if(this.aspeed) {
    this.pspeed=this.center.MG/(this.aspeed*this.a);
    this.perigey=2*this.center.MG/(this.pspeed*(this.pspeed+this.aspeed));
    this.apogey=2*this.a-this.perigey;
    this.excentrisitet=(this.a-this.perigey)/this.a;
    this.b=this.a*Math.sqrt(1-sqr(this.excentrisitet));
  } else if(this.pspeed) {
    this.aspeed=this.center.MG/(this.pspeed*this.a);
    this.perigey=2*this.center.MG/(this.pspeed*(this.pspeed+this.aspeed));
    this.apogey=2*this.a-this.perigey;
    this.excentrisitet=(this.a-this.perigey)/this.a;
    this.b=this.a*Math.sqrt(1-sqr(this.excentrisitet));
  }
}

function setSmallSizeEllipse(b) {
  this.b=b;
  if(this.a) {
    this.perigey=this.a-Math.sqrt(sqr(this.a)-sqr(this.b));
    this.apogey=2*this.a-this.perigey;
    this.excentrisitet=(this.apogey-this.perigey)/(this.apogey+this.perigey);
    this.pspeed=Math.sqrt(orbit.center.MG*orbit.apogey/orbit.a/orbit.perigey);
    this.aspeed=Math.sqrt(orbit.center.MG*orbit.perigey/orbit.a/orbit.apogey);
  } else if(this.excentrisitet) {
    this.a=this.b/Math.sqrt(1-sqr(this.excentrisitet));
    this.period=2*Math.PI*this.a*Math.sqrt(this.a/this.center.MG);
    this.perigey=this.a-Math.sqrt(sqr(this.a)-sqr(this.b));
    this.apogey=2*this.a-this.perigey;
    this.pspeed=Math.sqrt(orbit.center.MG*orbit.apogey/orbit.a/orbit.perigey);
    this.aspeed=Math.sqrt(orbit.center.MG*orbit.perigey/orbit.a/orbit.apogey);
  } else if(this.perigey) {
    this.apogey=sqr(this.b)/this.perigey;
    this.a=(this.apogey+this.perigey)/2;
    this.excentrisitet=(this.a-this.perigey)/this.a;
    this.period=2*Math.PI*this.a*Math.sqrt(this.a/this.center.MG);
    this.pspeed=Math.sqrt(orbit.center.MG*orbit.apogey/orbit.a/orbit.perigey);
    this.aspeed=Math.sqrt(orbit.center.MG*orbit.perigey/orbit.a/orbit.apogey);
  } else if(this.apogey) {
    this.perigey=sqr(b)/this.apogey;
    this.a=(this.perigey+this.apogey)/2;
    this.excentrisitet=(this.a-this.perigey)/this.a;
    this.period=2*Math.PI*this.a*Math.sqrt(this.a/this.center.MG);
    this.pspeed=Math.sqrt(orbit.center.MG*orbit.apogey/orbit.a/orbit.perigey);
    this.aspeed=Math.sqrt(orbit.center.MG*orbit.perigey/orbit.a/orbit.apogey);
  } else if(this.pspeed) {
    this.pspeed=null;
  } else if(this.aspeed) {
    this.aspeed=null;
  }
}

function setExcentrisitetEllipse(e) {
  this.excentrisitet=e;
  if(this.a) {
    this.perigey=this.a*(1-this.excentrisitet);
    this.apogey=this.a*(1+this.excentrisitet);
    this.b=this.a*Math.sqrt(1-sqr(this.excentrisitet));
    this.pspeed=Math.sqrt(orbit.center.MG*orbit.apogey/orbit.a/orbit.perigey);
    this.aspeed=Math.sqrt(orbit.center.MG*orbit.perigey/orbit.a/orbit.apogey);
  } else if(this.b) {
    this.a=this.b/Math.sqrt(1-sqr(this.excentrisitet));
    this.period=2*Math.PI*this.a*Math.sqrt(this.a/this.center.MG);
    this.perigey=this.a-Math.sqrt(sqr(this.a)-sqr(this.b));
    this.apogey=2*this.a-this.perigey;
    this.pspeed=Math.sqrt(orbit.center.MG*orbit.apogey/orbit.a/orbit.perigey);
    this.aspeed=Math.sqrt(orbit.center.MG*orbit.perigey/orbit.a/orbit.apogey);
  } else if(this.perigey) {
    this.apogey=this.perigey*(1+this.excentrisitet)/(1-this.excentrisitet);
    this.a=(this.perigey+this.apogey)/2;
    this.period=2*Math.PI*this.a*Math.sqrt(this.a/this.center.MG);
    this.b=this.a*Math.sqrt(1-sqr(this.excentrisitet));
    this.pspeed=Math.sqrt(orbit.center.MG*orbit.apogey/orbit.a/orbit.perigey);
    this.aspeed=Math.sqrt(orbit.center.MG*orbit.perigey/orbit.a/orbit.apogey);
  } else if(this.apogey) {
    this.perigey=this.apogey*(1-this.excentrisitet)/(1+this.excentrisitet);
    this.a=(this.perigey+this.apogey)/2;
    this.period=2*Math.PI*this.a*Math.sqrt(this.a/this.center.MG);
    this.b=this.a*Math.sqrt(1-sqr(this.excentrisitet));
    this.pspeed=Math.sqrt(orbit.center.MG*orbit.apogey/orbit.a/orbit.perigey);
    this.aspeed=Math.sqrt(orbit.center.MG*orbit.perigey/orbit.a/orbit.apogey);
  } else if(this.pspeed) {
    this.aspeed=this.pspeed*(1-this.excentrisitet)/(1+this.excentrisitet);
    this.a=this.center.MG/(this.pspeed*this.aspeed);
    this.period=2*Math.PI*this.a*Math.sqrt(this.a/this.center.MG);
    this.perigey=2*this.a*this.aspeed/(this.pspeed+this.aspeed);
    this.apogey=2*this.a-this.perigey;
    this.b=this.a*Math.sqrt(1-sqr(this.excentrisitet));
  } else if(this.aspeed) {
    this.pspeed=this.aspeed*(1+this.excentrisitet)/(1-this.excentrisitet);
    this.a=this.center.MG/(this.pspeed*this.aspeed);
    this.period=2*Math.PI*this.a*Math.sqrt(this.a/this.center.MG);
    this.perigey=2*this.a*this.aspeed/(this.pspeed+this.aspeed);
    this.apogey=2*this.a-this.perigey;
    this.b=this.a*Math.sqrt(1-sqr(this.excentrisitet));
  }
}

function setPspEllipse(s) {
  this.pspeed=s;
  if(this.perigey) {
    this.apogey=sqr(this.perigey)/(2*this.center.MG/sqr(this.pspeed)-this.perigey);
    this.a=(sqr(this.b)/this.perigey+this.perigey)/2;
    this.b=this.a*Math.sqrt(1-sqr(this.excentrisitet));
    this.period=2*Math.PI*this.a*Math.sqrt(this.a/this.center.MG);
    this.aspeed=this.pspeed*this.perigey/this.apogey;
  } else if(this.apogey) {
    this.perigey=Math.sqrt(sqr(this.apogey)/2+2*this.center.MG*this.apogey/sqr(this.pspeed))-this.apogey/2;
    this.a=(this.perigey+this.apogey)/2;
    this.excentrisitet=(this.a-this.perigey)/this.a;
    this.period=2*Math.PI*this.a*Math.sqrt(this.a/this.center.MG);
    this.aspeed=this.pspeed*this.perigey/this.apogey;
  } else if(this.a) {
    this.aspeed=this.center.MG/(this.pspeed*this.a);
    this.perigey=2*this.center.MG/(this.pspeed*(this.pspeed+this.aspeed));
    this.apogey=2*this.a-this.perigey;
    this.excentrisitet=(this.a-this.perigey)/this.a;
    this.b=this.a*Math.sqrt(1-sqr(this.excentrisitet));
  } else if(this.b) {
    this.b=null;
  } else if(this.excentrisitet) {
    this.aspeed=this.pspeed*(1-this.excentrisitet)/(1+this.excentrisitet);
    this.a=this.center.MG/(this.pspeed*this.aspeed);
    this.period=2*Math.PI*this.a*Math.sqrt(this.a/this.center.MG);
    this.perigey=2*this.a*this.aspeed/(this.pspeed+this.aspeed);
    this.apogey=2*this.a-this.perigey;
    this.b=this.a*Math.sqrt(1-sqr(this.excentrisitet));
  } else if(this.aspeed) {
    this.a=this.center.MG/(this.pspeed*this.aspeed);
    this.period=2*Math.PI*this.a*Math.sqrt(this.a/this.center.MG);
    this.excentrisitet=(this.pspeed-this.aspeed)/(this.pspeed+this.aspeed);
    this.perigey=this.a*(1-this.excentrisitet);
    this.apogey=this.a*(1+this.excentrisitet);
    this.b=this.a*Math.sqrt(1-sqr(this.excentrisitet));
  }
}

function setAspEllipse(s) {
  this.aspeed=s;
  if(this.apogey) {
    this.perigey=sqr(this.apogey)/(2*this.center.MG/sqr(this.aspeed)-this.apogey);
    this.a=(this.perigey+this.apogey)/2;
    this.excentrisitet=(this.a-this.perigey)/this.a;
    this.period=2*Math.PI*this.a*Math.sqrt(this.a/this.center.MG);
    this.pspeed=this.aspeed*this.apogey/this.perigey;
  } else if(this.perigey) {
    this.apogey=Math.sqrt(sqr(this.perigey)/2+2*this.center.MG*this.perigey/sqr(this.aspeed))-this.perigey/2;
    this.a=(this.perigey+this.apogey)/2;
    this.excentrisitet=(this.a-this.perigey)/this.a;
    this.period=2*Math.PI*this.a*Math.sqrt(this.a/this.center.MG);
    this.pspeed=this.aspeed*this.apogey/this.perigey;
  } else if(this.a) {
    this.pspeed=this.center.MG/(this.aspeed*this.a);
    this.perigey=2*this.center.MG/(this.pspeed*(this.pspeed+this.aspeed));
    this.apogey=2*this.a-this.perigey;
    this.excentrisitet=(this.a-this.perigey)/this.a;
    this.b=this.a*Math.sqrt(1-sqr(this.excentrisitet));
  } else if(this.b) {
    this.b=null;
  } else if(this.excentrisitet) {
    this.pspeed=this.aspeed*(1+this.excentrisitet)/(1-this.excentrisitet);
    this.a=this.center.MG/(this.pspeed*this.aspeed);
    this.period=2*Math.PI*this.a*Math.sqrt(this.a/this.center.MG);
    this.perigey=2*this.a*this.aspeed/(this.pspeed+this.aspeed);
    this.apogey=2*this.a-this.perigey;
    this.b=this.a*Math.sqrt(1-sqr(this.excentrisitet));
  } else if(this.pspeed) {
    this.a=this.center.MG/(this.pspeed*this.aspeed);
    this.period=2*Math.PI*this.a*Math.sqrt(this.a/this.center.MG);
    this.excentrisitet=(this.pspeed-this.aspeed)/(this.pspeed+this.aspeed);
    this.perigey=this.a*(1-this.excentrisitet);
    this.apogey=this.a*(1+this.excentrisitet);
    this.b=this.a*Math.sqrt(1-sqr(this.excentrisitet));
  }
}

function setPeriodEllipse(p) {
  this.period=p;
  this.a=Math.pow(this.center.MG,1./3)*Math.pow(this.period/(2*Math.PI),2./3);
  if(this.b) {
    this.perigey=this.a-Math.sqrt(sqr(this.a)-sqr(this.b));
    this.apogey=2*this.a-this.perigey;
    this.excentrisitet=(this.apogey-this.perigey)/(this.apogey+this.perigey);
    this.pspeed=Math.sqrt(orbit.center.MG*orbit.apogey/orbit.a/orbit.perigey);
    this.aspeed=Math.sqrt(orbit.center.MG*orbit.perigey/orbit.a/orbit.apogey);
  } else if(this.perigey) {
    this.apogey=this.a*2-this.perigey;
    this.excentrisitet=(this.a-this.perigey)/this.a;
    this.b=this.a*Math.sqrt(1-sqr(this.excentrisitet));
    this.pspeed=Math.sqrt(orbit.center.MG*orbit.apogey/orbit.a/orbit.perigey);
    this.aspeed=Math.sqrt(orbit.center.MG*orbit.perigey/orbit.a/orbit.apogey);
  } else if(this.apogey) {
    this.perigey=2*this.a-this.apogey;
    this.excentrisitet=(this.a-this.perigey)/this.a;
    this.b=this.a*Math.sqrt(1-sqr(this.excentrisitet));
    this.pspeed=Math.sqrt(orbit.center.MG*orbit.apogey/orbit.a/orbit.perigey);
    this.aspeed=Math.sqrt(orbit.center.MG*orbit.perigey/orbit.a/orbit.apogey);
  } else if(this.excentrisitet) {
    this.perigey=this.a*(1-this.excentrisitet);
    this.apogey=this.a*(1+this.excentrisitet);
    this.b=this.a*Math.sqrt(1-sqr(this.excentrisitet));
    this.pspeed=Math.sqrt(orbit.center.MG*orbit.apogey/orbit.a/orbit.perigey);
    this.aspeed=Math.sqrt(orbit.center.MG*orbit.perigey/orbit.a/orbit.apogey);
  } else if(this.aspeed) {
    this.pspeed=this.center.MG/(this.aspeed*this.a);
    this.perigey=2*this.center.MG/(this.pspeed*(this.pspeed+this.aspeed));
    this.apogey=2*this.a-this.perigey;
    this.excentrisitet=(this.a-this.perigey)/this.a;
    this.b=this.a*Math.sqrt(1-sqr(this.excentrisitet));
  } else if(this.pspeed) {
    this.aspeed=this.center.MG/(this.pspeed*this.a);
    this.perigey=2*this.center.MG/(this.pspeed*(this.pspeed+this.aspeed));
    this.apogey=2*this.a-this.perigey;
    this.excentrisitet=(this.a-this.perigey)/this.a;
    this.b=this.a*Math.sqrt(1-sqr(this.excentrisitet));
  }
}

function cleanEllipse() {
  this.perigey=null;
  this.apogey=null;
  this.a=null;
  this.b=null;
  this.excentrisitet=null;
  this.period=null;
  this.pspeed=null;
  this.aspeed=null;
}

function Ellipse2Circle() {
  var c=new CircleOrbit(this.center);
  if(this.a)
    c.setRadius(this.a);
  else if(this.b)
    c.setRadius(this.b);
  else if(this.perigey)
    c.setRadius(this.perigey);
  else if(this.apogey)
    c.setRadius(this.apogey);
  else if(this.pspeed)
    c.setSpeed(this.pspeed);
  else if(this.aspeed)
    c.setSpeed(this.aspeed);
  return c;
}

function Ellipse2Ellipse() {
  return this;
}

function Ellipse2Parabolic() {
  var p=new ParabolicOrbit(this.center);
  if(this.perigey)
    p.setPerigey(this.perigey);
  else if(this.apogey)
    p.setPerigey(this.apogey);
  else if(this.pspeed)
    p.setPsp(this.pspeed);
  else if(this.aspeed)
    p.setPsp(this.aspeed);
  return p;
}

function Ellipse2Hyperbolic() {
  var h=new HyperbolicOrbit(this.center);
  if(this.perigey)
    h.setPerigey(this.perigey);
  else if(this.apogey)
    h.setPerigey(this.apogey);
  else if(this.pspeed)
    h.setPsp(this.pspeed);
  else if(this.aspeed)
    h.setIsp(this.aspeed);
  if(this.excentrisitet)
    if(this.excentrisitet<1)
      h.setExcentrisitet(1./this.excentrisitet);
    else
      h.setExcentrisitet(this.excentrisitet);
  return h;
}

function CheckEllipse() {
  if(this.perigey)
    if(this.perigey<this.center.radius)
      return false;
    else if(this.apogey && this.perigey>this.apogey)
      return false;
  if(this.excentrisitet && (this.excentrisitet<0 || this.excentrisitet>=1.))
    return false;
  if(this.a && this.a<this.center.radius)
    return false;
  if(this.b && this.b<this.center.radius)
    return false;
  return true;
}

function EllipseOrbit(b) {
  this.center=b;
  this.setPerigey=setPerigeyEllipse;
  this.setApogey=setApogeyEllipse;
  this.setBigSize=setBigSizeEllise;
  this.setSmallSize=setSmallSizeEllipse;
  this.setExcentrisitet=setExcentrisitetEllipse;
  this.setPsp=setPspEllipse;
  this.setAsp=setAspEllipse;
  this.clean=cleanEllipse;
  this.circle=Ellipse2Circle;
  this.ellipse=Ellipse2Ellipse;
  this.parabolic=Ellipse2Parabolic;
  this.hyperbolic=Ellipse2Hyperbolic;
  this.check=CheckEllipse;
}

function setPerigeyParabolic(p) {
  this.perigey=p;
  this.pspeed=Math.sqrt(2*this.center.MG/this.perigey);
}

function setPspParabolic(s) {
  this.pspeed=s;
  this.perigey=2*this.center.MG/(this.perigey*this.perigey);
}

function cleanParabolic() {
  this.perigey=null;
  this.pspeed=null;
}

function Parabolic2Circle() {
  var c=new CircleOrbit(this.center);
  if(this.perigey)
    c.setRadius(this.perigey);
  return c;
}

function Parabolic2Ellipse() {
  var e=new EllipseOrbit(this.center);
  if(this.perigey)
    e.setPerigey(this.perigey);
  return e;
}

function Parabolic2Parabolic() {
  return this;
}

function Parabolic2Hyperbolic() {
  var h=new HyperbolicOrbit(this.center);
  if(this.perigey)
    h.setPerigey(this.perigey);
  return h;
}

function CheckParabolic() {
  if(this.perigey && this.perigey<this.center.radius)
    return false;
  return true;
}

function ParabolicOrbit(b) {
  this.center=b;
  this.setPerigey=setPerigeyParabolic;
  this.setPsp=setPspParabolic;
  this.clean=cleanParabolic;
  this.circle=Parabolic2Circle;
  this.ellipse=Parabolic2Ellipse;
  this.parabolic=Parabolic2Parabolic;
  this.hyperbolic=Parabolic2Hyperbolic;
  this.check=CheckParabolic;
}

function setPerigeyHyperbolic(p) {
  this.perigey=p;
  if(this.pspeed) {
    this.ispeed=Math.sqrt(sqr(this.pspeed)-2*this.center.MG/this.perigey);
    this.excentrisitet=(sqr(this.pspeed)+sqr(this.ispeed))/(sqr(this.pspeed)-sqr(this.ispeed));
    this.turn=2*Math.asin(1./this.excentrisitet);
    this.d=this.perigey*this.pspeed/this.ispeed;
  } else if(this.ispeed) {
    this.pspeed=Math.sqrt(sqr(this.ispeed)+2*this.center.MG/this.perigey);
    this.excentrisitet=(sqr(this.pspeed)+sqr(this.ispeed))/(sqr(this.pspeed)-sqr(this.ispeed));
    this.turn=2*Math.asin(1./this.excentrisitet);
    this.d=this.perigey*this.pspeed/this.ispeed;
  } else if(this.turn) {
    this.d=this.perigey*Math.sqrt((this.excentrisitet+1)/(this.excentrisitet-1));
    this.pspeed=Math.sqrt(this.center.MG*(this.excentrisitet+1)/this.perigey);
    this.ispeed=Math.sqrt(this.center.MG*(this.excentrisitet-1)/this.perigey);
    this.d=this.perigey*this.pspeed/this.ispeed;
  } else if(this.d) {
    this.excentrisitet=(sqr(this.d)+sqr(this.perigey))/(sqr(this.d)-sqr(this.perigey));
    this.turn=2*Math.asin(1./this.excentrisitet);
    this.pspeed=Math.sqrt(this.center.MG*(this.excentrisitet+1)/this.perigey);
    this.ispeed=Math.sqrt(this.center.MG*(this.excentrisitet-1)/this.perigey);
  }
}

function setPspHyperbolic(s) {
  this.pspeed=s;
  if(this.perigey) {
    this.ispeed=Math.sqrt(sqr(this.pspeed)-2*this.center.MG/this.perigey);
    this.excentrisitet=(sqr(this.pspeed)+sqr(this.ispeed))/(sqr(this.pspeed)-sqr(this.ispeed));
    this.turn=2*Math.asin(1./this.excentrisitet);
    this.d=this.perigey*this.pspeed/this.ispeed;
  } else if(this.ispeed) {
    this.perigey=2*this.center.MG/(sqr(this.pspeed)-sqr(this.ispeed));
    this.d=this.perigey*this.pspeed/this.ispeed;
    this.excentrisitet=(sqr(this.pspeed)+sqr(this.ispeed))/(sqr(this.pspeed)-sqr(this.ispeed));
    this.turn=2*Math.asin(1./this.excentrisitet);
  } else if(this.turn) {
    this.ispeed=this.pspeed*Math.sqrt((this.excentrisitet-1)/(this.excentrisitet+1));
    this.perigey=2*this.center.MG/(sqr(this.pspeed)-sqr(this.ispeed));
    this.d=this.perigey*this.pspeed/this.ispeed;
  } else if(this.d) {
    this.d=null;
  }
}

function setIspHyperbolic(s) {
  this.ispeed=s;
  if(this.perigey) {
    this.pspeed=Math.sqrt(sqr(this.ispeed)+2*this.center.MG/this.perigey);
    this.excentrisitet=(sqr(this.pspeed)+sqr(this.ispeed))/(sqr(this.pspeed)-sqr(this.ispeed));
    this.turn=2*Math.asin(1./this.excentrisitet);
    this.d=this.perigey*this.pspeed/this.ispeed;
  } else if(this.pspeed) {
    this.perigey=2*this.center.MG/(sqr(this.pspeed)-sqr(this.ispeed));
    this.d=this.perigey*this.pspeed/this.ispeed;
    this.excentrisitet=(sqr(this.pspeed)+sqr(this.ispeed))/(sqr(this.pspeed)-sqr(this.ispeed));
    this.turn=2*Math.asin(1./this.excentrisitet);
  } else if(this.turn) {
    this.pspeed=this.ispeed*Math.sqrt((this.excentrisitet+1)/(this.excentrisitet-1));
    this.perigey=2*this.center.MG/(sqr(this.pspeed)-sqr(this.ispeed));
    this.d=this.perigey*this.pspeed/this.ispeed;
  } else if(this.d) {
    var f=this.center.MG/(this.ispeed*this.d);
    this.pspeed=f+Math.sqrt(sqr(f)+sqr(this.ispeed));
    this.perigey=2*this.center.MG/(sqr(this.pspeed)-sqr(this.ispeed));
    this.excentrisitet=(sqr(this.pspeed)+sqr(this.ispeed))/(sqr(this.pspeed)-sqr(this.ispeed));
    this.turn=2*Math.asin(1./this.excentrisitet);
  }
}

function setTurnHyperbolic(a) {
  this.turn=a;
  this.excentrisitet=1./Math.sin(this.turn/2);
  if(this.perigey) {
    this.d=this.perigey*Math.sqrt((this.excentrisitet+1)/(this.excentrisitet-1));
    this.pspeed=Math.sqrt(this.center.MG*(this.excentrisitet+1)/this.perigey);
    this.ispeed=Math.sqrt(this.center.MG*(this.excentrisitet-1)/this.perigey);
    this.d=this.perigey*this.pspeed/this.ispeed;
  } else if(this.pspeed) {
    this.ispeed=this.pspeed*Math.sqrt((this.excentrisitet-1)/(this.excentrisitet+1));
    this.perigey=2*this.center.MG/(sqr(this.pspeed)-sqr(this.ispeed));
    this.d=this.perigey*this.pspeed/this.ispeed;
  } else if(this.ispeed) {
    this.pspeed=this.ispeed*Math.sqrt((this.excentrisitet+1)/(this.excentrisitet-1));
    this.perigey=2*this.center.MG/(sqr(this.pspeed)-sqr(this.ispeed));
    this.d=this.perigey*this.pspeed/this.ispeed;
  } else if(this.d) {
    this.perigey=this.d*Math.sqrt((this.excentrisitet-1)/(this.excentrisitet+1));
    this.pspeed=Math.sqrt(this.center.MG*(this.excentrisitet+1)/this.perigey);
    this.ispeed=Math.sqrt(this.center.MG*(this.excentrisitet-1)/this.perigey);
  }
}

function setExcentrisitetHyperbolic(e) {
  this.excentrisitet=e;
  this.turn=2*Math.asin(1./this.excentrisitet);
  if(this.perigey) {
    this.d=this.perigey*Math.sqrt((this.excentrisitet+1)/(this.excentrisitet-1));
    this.pspeed=Math.sqrt(this.center.MG*(this.excentrisitet+1)/this.perigey);
    this.ispeed=Math.sqrt(this.center.MG*(this.excentrisitet-1)/this.perigey);
    this.d=this.perigey*this.pspeed/this.ispeed;
  } else if(this.pspeed) {
    this.ispeed=this.pspeed*Math.sqrt((this.excentrisitet-1)/(this.excentrisitet+1));
    this.perigey=2*this.center.MG/(sqr(this.pspeed)-sqr(this.ispeed));
    this.d=this.perigey*this.pspeed/this.ispeed;
  } else if(this.ispeed) {
  } else if(this.d) {
    this.perigey=this.d*Math.sqrt((this.excentrisitet-1)/(this.excentrisitet+1));
    this.pspeed=Math.sqrt(this.center.MG*(this.excentrisitet+1)/this.perigey);
    this.ispeed=Math.sqrt(this.center.MG*(this.excentrisitet-1)/this.perigey);
  }
}

function setDHyperbolic(d) {
  this.d=d;
  if(this.perigey) {
    this.excentrisitet=(sqr(this.d)+sqr(this.perigey))/(sqr(this.d)-sqr(this.perigey));
    this.turn=2*Math.asin(1./this.excentrisitet);
    this.pspeed=Math.sqrt(this.center.MG*(this.excentrisitet+1)/this.perigey);
    this.ispeed=Math.sqrt(this.center.MG*(this.excentrisitet-1)/this.perigey);
  } else if(this.pspeed) {
    this.pspeed=null;
  } else if(this.ispeed) {
    var f=this.center.MG/(this.ispeed*this.d);
    this.pspeed=f+Math.sqrt(sqr(f)+sqr(this.ispeed));
    this.perigey=2*this.center.MG/(sqr(this.pspeed)-sqr(this.ispeed));
    this.excentrisitet=(sqr(this.pspeed)+sqr(this.ispeed))/(sqr(this.pspeed)-sqr(this.ispeed));
    this.turn=2*Math.asin(1./this.excentrisitet);
  } else if(this.excentrisitet) {
    this.perigey=this.d*Math.sqrt((this.excentrisitet-1)/(this.excentrisitet+1));
    this.pspeed=Math.sqrt(this.center.MG*(this.excentrisitet+1)/this.perigey);
    this.ispeed=Math.sqrt(this.center.MG*(this.excentrisitet-1)/this.perigey);
  }
}

function cleanHyperbolic() {
  this.perigey=null;
  this.pspeed=null;
  this.ispeed=null;
  this.turn=null;
  this.excentrisitet=null;
  this.d=null;
}

function Hyperbolic2Circle() {
  var c=new CircleOrbit(this.center);
  if(this.perigey)
    c.setRadius(this.perigey);
  else if(this.pspeed)
    c.setSpeed(this.pspeed);
  return c;
}

function Hyperbolic2Ellipse() {
  var e=new EllipseOrbit(this.center);
  if(this.perigey)
    e.setPerigey(this.perigey);
  if(this.excentrisitet)
    e.setExcentrisitet(1./this.excentrisitet);
  else if(this.pspeed)
    e.setPsp(this.pspeed);
  return e;
}

function Hyperbolic2Parabolic() {
  var p=new ParabolicOrbit(this.center);
  if(this.perigey)
    p.setPerigey(this.perigey);
  else if(this.pspeed)
    p.setPsp(this.pspeed);
  return p;
}

function Hyperbolic2Hyperbolic() {
  return this;
}

function CheckHyperbolic() {
  if(this.perigey && this.perigey<this.center.radius)
    return false;
  if(this.excentrisitet && this.excentrisitet<1.)
    return false;
  if(this.pspeed && this.pspeed<=0)
    return false;
  if(this.ispeed && this.ispeed<0)
    return false;
  if(this.turn && (this.turn<0 || this.turn>Math.PI))
    return false;
  return true;
}

function HyperbolicOrbit(b) {
  this.center=b;
  this.setPerigey=setPerigeyHyperbolic;
  this.setPsp=setPspHyperbolic;
  this.setIsp=setIspHyperbolic;
  this.setTurn=setTurnHyperbolic;
  this.setExctntrisitet=setExcentrisitetHyperbolic;
  this.setD=setDHyperbolic;
  this.circle=Hyperbolic2Circle;
  this.ellipse=Hyperbolic2Ellipse;
  this.parabolic=Hyperbolic2Parabolic;
  this.hyperbolic=Hyperbolic2Hyperbolic;
  this.clean=cleanHyperbolic;
  this.check=CheckHyperbolic;
}

function selectBody() {
  var b=document.getElementById('cbody').value, n;
  for(n=0; n<bodies.length; n++)
    if(b==bodies[n].name) break;
  if(n==bodies.length) return false;
  curbody=bodies[n];
  if(orbit) {
    orbit.center=curbody;
    orbit.clean();
  }
  if(point) {
    point.center=curbody;
    point.clean();
  }
  var t=document.getElementById('propetyes');
  t.rows[0].cells[1].innerHTML=curbody.mass/document.getElementById('munit').value;
  t.rows[1].cells[1].innerHTML=curbody.radius/document.getElementById('runit').value;
  t.rows[2].cells[1].innerHTML=curbody.period/document.getElementById('punit').value;
  return true;
}

function selectType(t) {
  var i;
  document.getElementById('ortype').value=t;
  for(i=0; i<orbitType.length; i++)
    document.getElementById(orbitType[i]).style.display=(orbitType[i]==t)?'':'none';
  switch(t) {
    case 'circle':
      if(orbit)
        orbit=orbit.circle();
      else if(curbody)
        orbit=new CircleOrbit(curbody);
      showCircle();
      break;
    case 'ellipse':
      if(orbit)
        orbit=orbit.ellipse();
      else if(curbody)
        orbit=new EllipseOrbit(curbody);
      showEllipse();
      break;
    case 'hyperbolic':
      if(orbit)
        orbit=orbit.hyperbolic();
      else if(curbody)
        orbit=new HyperbolicOrbit(curbody);
      showHyperbolic();
      break;
    case 'parabolic':
      if(orbit)
        orbit=orbit.parabolic();
      else if(curbody)
        orbit=new ParabolicOrbit(curbody);
      showParabolic();
      break;
  }
}

function initial() {
  selectBody();
  point=new Point(curbody);
  selectType(document.getElementById('ortype').value);
}

function showCircle() {
  if(orbit.radius) {
    document.getElementById('crad').value=orbit.radius/document.getElementById('crunit').value;
    document.getElementById('chei').value=(orbit.radius-orbit.center.radius)/document.getElementById('chunit').value;
  }
  else {
    document.getElementById('crad').value='';
    document.getElementById('chei').value='';
  }
  if(orbit.period)
    document.getElementById('cper').value=orbit.period/document.getElementById('cpunit').value;
  else
    document.getElementById('cper').value='';
  if(orbit.speed)
    document.getElementById('cspd').value=orbit.speed/document.getElementById('cspunit').value;
  else
    document.getElementById('cspd').value='';
  document.getElementById('msg').innerHTML=orbit.check()?'':'Недопустимые параметры!';
}

function c_hei(h) {
  orbit.setRadius(h*document.getElementById('crunit').value+orbit.center.radius);
  showCircle();
}

function c_rad(r) {
  r*=document.getElementById('crunit').value;
  orbit.setRadius(r);
  showCircle();
}

function c_per(p) {
  p*=document.getElementById('cpunit').value;
  orbit.setPeriod(p);
  showCircle();
}

function c_sp(s) {
  s*=document.getElementById('cspunit').value;
  orbit.setSpeed(s);
  showCircle();
}

function showParabolic() {
  if(orbit.pspeed)
    document.getElementById('ppsp').value=orbit.pspeed/document.getElementById('ppspunit').value;
  else
    document.getElementById('ppsp').value='';
  if(orbit.perigey)
    document.getElementById('pper').value=(orbit.perigey-orbit.center.radius)/document.getElementById('ppunit').value;
  else
    document.getElementById('pper').value='';
  document.getElementById('msg').innerHTML=orbit.check()?'':'Недопустимые параметры!';
}

function p_per(p) {
  orbit.setPerigey(p*document.getElementById('ppunit').value- -orbit.center.radius);
  showParabolic();
}

function p_psp(s) {
  orbit.setPsp(s*document.getElementById('ppspunit').value);
  showParabolic();
}

function showEllipse() {
  if(orbit.perigey)
    document.getElementById('eper').value=(orbit.perigey-orbit.center.radius)/document.getElementById('epunit').value;
  else
    document.getElementById('eper').value='';
  if(orbit.apogey)
    document.getElementById('eapo').value=(orbit.apogey-orbit.center.radius)/document.getElementById('eaunit').value;
  else
    document.getElementById('eapo').value='';
  if(orbit.a)
    document.getElementById('ehalf').value=orbit.a/document.getElementById('ehunit').value;
  else
    document.getElementById('ehalf').value='';
  if(orbit.b)
    document.getElementById('esmall').value=orbit.b/document.getElementById('esmunit').value;
  else
    document.getElementById('esmall').value='';
  if(orbit.excentrisitet)
    document.getElementById('eexc').value=orbit.excentrisitet;
  else
    document.getElementById('eexc').value='';
  if(orbit.pspeed)
    document.getElementById('epsp').value=orbit.pspeed/document.getElementById('epspunit').value;
  else
    document.getElementById('epsp').value='';
  if(orbit.aspeed)
    document.getElementById('easp').value=orbit.aspeed/document.getElementById('easpunit').value;
  else
    document.getElementById('easp').value='';
  if(orbit.period)
    document.getElementById('eperiod').value=orbit.period/document.getElementById('etunit').value;
  else
    document.getElementById('eperiod').value='';
  document.getElementById('msg').innerHTML=orbit.check()?'':'Недопустимые параметры!';
}

function e_per(p) {
  orbit.setPerigey(p*document.getElementById('epunit').value+orbit.center.radius);
  showEllipse();
}

function e_apo(a) {
  orbit.setApogey(a*document.getElementById('eaunit').value+orbit.center.radius);
  showEllipse();
}

function e_half(a) {
  orbit.setBigSize(a*document.getElementById('ehunit').value);
  showEllipse();
}

function e_small(b) {
  orbit.setSmallSize(b*document.getElementById('esmunit').value);
  showEllipse();
}

function e_exc(e) {
  orbit.setExcentrisitetEllipse(e);
  showEllipse();
}

function e_psp(s) {
  orbit.setPsp(s*document.getElementById('epspunit').value);
  showEllipse();
}

function e_asp(s) {
  orbit.setAsp(s*document.getElementById('easpunit').value);
  showEllipse();
}

function e_period(t) {
  orbit.setPeriod(t*document.getElementById('etunit').value);
  showEllipse();
}

function showHyperbolic() {
  if(orbit.perigey)
    document.getElementById('hper').value=(orbit.perigey-orbit.center.radius)/document.getElementById('hpunit').value;
  else
    document.getElementById('hper').value='';
  if(orbit.pspeed)
    document.getElementById('hpsp').value=orbit.pspeed/document.getElementById('hpsunit').value;
  else
    document.getElementById('hpsp').value='';
  if(orbit.ispeed)
    document.getElementById('hisp').value=orbit.ispeed/document.getElementById('hisunit').value;
  else
    document.getElementById('hisp').value='';
  if(orbit.turn)
    document.getElementById('hturn').value=orbit.turn/document.getElementById('htunit').value;
  else
    document.getElementById('hturn').value='';
  if(orbit.excentrisitet)
    document.getElementById('hexc').value=orbit.excentrisitet;
  else
    document.getElementById('hexc').value='';
  if(orbit.d)
    document.getElementById('hd').value=orbit.d/document.getElementById('hdunit').value;
  else
    document.getElementById('hd').value='';
  document.getElementById('msg').innerHTML=orbit.check()?'':'Недопустимые параметры!';
}

function h_per(p) {
  orbit.setPerigey(p*document.getElementById('hpunit').value- -orbit.center.radius);
  showHyperbolic();
}

function h_psp(s) {
  orbit.setPsp(s*document.getElementById('hpsunit').value);
  showHyperbolic();
}

function h_isp(s) {
  orbit.setIsp(s*document.getElementById('hisunit').value);
  showHyperbolic();
}

function h_turn(a) {
  orbit.setTurn(a*document.getElementById('htunit').value);
  showHyperbolic();
}

function h_exc(e) {
  orbit.setExcentrisitet(e);
  showHyperbolic();
}

function h_d(d) {
  orbit.setD(d*document.getElementById('hdunit').value);
  showHyperbolic();
}

function pcalc() {
  var speed2=Math.sqrt(2*this.center.MG/this.distance);
  if(this.distance && this.speed && orbit) {
    if(this.speed<speed2) {
      selectType('ellipse');
      orbit.clean();
      orbit.setBigSize(1./(2./this.distance-sqr(this.speed)/this.center.MG));
      if(this.tspeed) {
        orbit.setPerigey(sqr(this.distance*this.tspeed)/(this.center.MG+Math.sqrt(sqr(this.center.MG)-sqr(this.distance*this.tspeed)*(sqr(speed2)-sqr(this.speed)))));
      }
      showEllipse();
    } else if(this.speed==speed2) {
      selectType('parabolic');
      orbit.clean();
      if(this.tspeed) {
        orbit.setPerigey(sqr(this.distance*this.tspeed)/(this.center.MG+Math.sqrt(sqr(this.centre.MG)-sqr(this.distance*this.tspeed)*(sqr(speed2)-sqr(this.speed)))));
      }
      showParabolic();
    } else {
      selectType('hyperbolic');
      orbit.clean();
      orbit.setIsp(Math.sqrt(sqr(this.speed)-sqr(speed2)));
      if(this.tspeed) {
        orbit.setPerigey(sqr(this.distance*this.tspeed)/(this.center.MG+Math.sqrt(sqr(this.center.MG)-sqr(this.distance*this.tspeed)*(sqr(speed2)-sqr(this.speed)))));
      }
      showHyperbolic();
    }
  }
}

function setpdist(v) {
  this.distance=v;
  if(this.pitch)
    this.aiming=this.distance*Math.abs(Math.cos(this.pitch));
  else if(this.aiming) {
    this.pitch=Math.acos(this.aiming/this.distance);
    if(this.speed) {
      this.nspeed=this.speed*Math.sin(this.pitch);
      this.tspeed=this.speed*Math.cos(this.pitch);
    } else if(this.nspeed) {
      this.speed=this.nspeed/Math.sin(this.pitch);
      this.tspeed=this.nspeed/Math.tan(this.pitch);
    } else if(this.tspeed) {
      this.speed=this.tspeed/Math.cos(this.pitch);
      this.nspeed=this.tspeed*Math.tan(this.pitch);
    }
  }
  ShowPoint(this);
  this.calc();
}

function setpaiming(v) {
  this.aiming=v;
  if(this.distance) {
    this.pitch=Math.acos(this.aiming/this.distance);
    if(this.speed) {
      this.nspeed=this.speed*Math.sin(this.pitch);
      this.tspeed=this.speed*Math.cos(this.pitch);
    } else if(this.nspeed) {
      this.speed=this.nspeed/Math.sin(this.pitch);
      this.tspeed=this.nspeed/Math.tan(this.pitch);
    } else if(this.tspeed) {
      this.speed=this.tspeed/Math.cos(this.pitch);
      this.nspeed=this.tspeed*Math.tan(this.pitch);
    }
  } else if(this.pitch) {
    this.distance=this.aiming/Math.cos(this.pitch);
  }
  ShowPoint(this);
  this.calc();
}

function setpspeed(v) {
  this.speed=v;
  if(this.pitch) {
    this.nspeed=this.speed*Math.sin(this.pitch);
    this.tspeed=this.speed*Math.cos(this.pitch);
  } else if(this.nspeed) {
    this.tspeed=Math.sqrt(sqr(this.speed)-sqr(this.nspeed));
    this.pitch=Math.asin(this.nspeed/this.speed);
    if(this.distance)
      this.aiming=this.distance*Math.abs(Math.cos(this.pitch));
    else if(this.aiming)
      this.distance=this.aiming/Math.abs(Math.cos(this.pitch));
  } else if(this.tspeed) {
    this.nspeed=Math.sqrt(sqr(this.speed)-sqr(this.tspeed));
    this.pitch=Math.acos(this.tspeed/this.speed);
    if(this.distance)
      this.aiming=this.distance*Math.abs(Math.cos(this.pitch));
    else if(this.aiming)
      this.distance=this.aiming/Math.abs(Math.cos(this.pitch));
  }
  ShowPoint(this);
  this.calc();
}

function setptsp(v) {
  this.tspeed=v;
  if(this.speed) {
    this.nspeed=Math.sqrt(sqr(this.speed)-sqr(this.tspeed));
    this.pitch=Math.acos(this.tspeed/this.speed);
    if(this.distance)
      this.aiming=this.distance*Math.abs(Math.cos(this.pitch));
    else if(this.aiming)
      this.distance=this.aiming/Math.abs(Math.cos(this.pitch));
  } else if(this.nspeed) {
    this.speed=Math.hypot(this.nspeed,this.tspeed);
    this.pitch=Math.atan2(this.nspeed,this.tspeed);
    if(this.distance)
      this.aiming=this.distance*Math.abs(Math.cos(this.pitch));
    else if(this.aiming)
      this.distance=this.aiming/Math.abs(Math.cos(this.pitch));
  } else if(this.pitch) {
    this.speed=this.tspeed/Math.cos(this.pitch);
    this.nspeed=this.tspeed*Math.tan(this.pitch);
  }
  ShowPoint(this);
  this.calc();
}

function setpnsp(v) {
  this.nspeed=v;
  if(this.speed) {
    this.tspeed=Math.sqrt(sqr(this.speed)-sqr(this.nspeed));
    this.pitch=Math.asin(this.nspeed/this.speed);
    if(this.distance)
      this.aiming=this.distance*Math.abs(Math.cos(this.pitch));
    else if(this.aiming)
      this.distance=this.aiming/Math.abs(Math.cos(this.pitch));
  } else if(this.pitch) {
    this.speed=this.nspeed/Math.sin(this.pitch);
    this.tspeed=this.nspeed/Math.tan(this.pitch);
  } else if(this.tspeed) {
    this.pitch=Math.atan2(this.nspeed,this.tspeed);
    this.speed=Math.hypot(this.nspeed,this.tspeed);
    if(this.distance)
      this.aiming=this.distance*Math.abs(Math.cos(this.pitch));
    else if(this.aiming)
      this.distance=this.aiming/Math.abs(Math.cos(this.pitch));
  }
  ShowPoint(this);
  this.calc();
}

function setpangle(v) {
  this.pitch=v;
  if(this.speed) {
    this.nspeed=this.speed*Math.sin(this.pitch);
    this.tspeed=this.speed*Math.cos(this.pitch);
  } else if(this.nspeed) {
    this.speed=this.nspeed/Math.sin(this.pitch);
    this.tspeed=this.nspeed/Matj.tan(this.pitch);
  } else if(this.tspeed) {
    this.speed=this.nspeed/Math.cos(this.pitch);
    this.nspeed=this.nspeed*Math.tan(this.pitch);
  }
  if(this.distance)
    this.aiming*Math.abs(Math.cos(this.pitch));
  else if(this.aiming)
    this.distance=this.aiming/Math.abs(Math.cos(this.pitch));
  ShowPoint(this);
  this.calc();
}

function pclean() {
  this.distance=null;
  this.aiming=null;
  this.speed=null;
  this.tspeed=null;
  this.nspeed=null;
  this.pitch=null;
}

function ShowPoint(p) {
  if(p.distance) {
    document.getElementById('pdist').value=p.distance/document.getElementById('pdunit').value;
    document.getElementById('pheight').value=(p.distance-p.center.radius)/document.getElementById('pdunit').value;
  }
  else
    document.getElementById('pdist').value='';
  if(p.aiming)
    document.getElementById('paiming').value=p.aiming/document.getElementById('paimunit').value;
  else
    document.getElementById('paiming').value='';
  if(p.speed)
    document.getElementById('psp').value=p.speed/document.getElementById('psunit').value;
  else 
    document.getElementById('psp').value='';
  if(p.tspeed)
    document.getElementById('ptsp').value=p.tspeed/document.getElementById('ptsunit').value;
  else
    document.getElementById('ptsp').value='';
  if(p.nspeed)
    document.getElementById('pnsp').value=p.nspeed/document.getElementById('pnsunit').value;
  else
    document.getElementById('pnsp').value='';
  if(p.pitch)
    document.getElementById('ppitch').value=p.pitch/document.getElementById('pptunit').value;
  else
    document.getElementById('ppitch').value='';
}

function Point(b) {
  this.center=b;
  this.setDist=setpdist;
  this.setAiming=setpaiming;
  this.setSpeed=setpspeed;
  this.setTSpeed=setptsp;
  this.setNSpeed=setpnsp;
  this.setPitch=setpangle;
  this.calc=pcalc;
  this.clean=pclean;
}

function dihotomia(a,b,f,acc) {
  var fa=f(a), fb=f(b);
  if(fa==0) return a;
  if(fb==0) return b;
  if(b-a<acc) return (a+b)/2;
  if(fa*fb>0) return null;
  do {
    var c=(a+b)/2, fc=f(c);
    if(fc==0) return c;
    if(fa*fc<0) b=c; else a=c;
  } while (b-a>acc);
  return (a+b)/2;
}

function findPoint(t) {
}

function chooseTab(n) {
  var m=document.getElementById('mainmenu'), i;
  for(i=0; i<m.rows[0].cells.length; ++i)
    if(i==n) {
      m.rows[0].cells[i].bgColor='lightblue';
      m.rows[0].cells[i].style.fontWeight='bold';
      document.getElementById('tab'+i).style.display='';
    }
    else {
      m.rows[0].cells[i].bgColor='lightgray';
      m.rows[0].cells[i].style.fontWeight='normal';
      document.getElementById('tab'+i).style.display='none';
    }
}
