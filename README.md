# metro-sim
This was my first "real" project. Back then, I was learning computer programming by myself, so I didn't know anything about good code practices.
The development of the project is over. The last main version of the code was released on May 2nd, 2020. I am the only contributor of this project, and it would be very difficult for me to get back into this project.

## Context
Back in 2016, I started by myself to learn computer programming and C++. At the end of the year, I started to learn 3D programming with the help of the library OpenGL.
As I was (and I am still) passionate by the Parisian metro, I built a metro driving simulator, that I wanted as realistic as possible. I coded a lot during my last year of high school ("Terminale") and my first year of university ("classe préparatoire"), but I had to much work to do the following year to continue the project, that is now more of less dead -- even though the last code modifications were done in January 2022.

## Description
The application is a simulator: the point is to drive a metro train in a line, while making passengers enter and leave the train, respecting speed limitations and avoiding to collide with the other trains of the line.
The simulation is endless. When you reach the last station of the line, you can continue driving and you will get in the other direction of the same line. When you complete the second direction, you can continue driving and you will get back to the starting point.

## Functionalities

### In a station
- Open and close the doors with real sound.
- Let the closing bell ring (at least 3 seconds and as much as you wany), with real sound (that will loop to accommodate any duration).
- Leave the train to see the passengers and check that everything is going well... but you need to reach back your driving spot to continue driving!)
- Make passengers leave the train (not rendered) and enter it (the passengers are waiting on the quay, you can see them enter the train). The passengers design is not something I spent a lot of time with, there are about 700 different designs possible for a passenger.
- When the train is almost full, the passengers need more time to enter the train because they have to find their way in!
- Some stations will host more passengers than others, and the frequentation will also depend on the direction (in the station before the last station, a lot of passengers will leave but only a few will take the metro for one station).
- The stations may be refurbished (following a 1-year cycle) and the advertisments on certain walls may change too.
- Each station has a unique ambiant light! Some stations are slightly lighter than others.
- In almost all the stations, the estimated waiting time is displayed on top, allowing the passengers to get real-time information. More time between a train and the previous one will also mean more passengers waiting for the train!
- When you stop at a station, you will see a panel indicating you whether you are ahead or behind schedule. In some stations, you also get help to determine the optimal time to leave the station with no extra delays; if you are late then it will suggest you to leave earlier to catch up with the timing and vice versa!

### Between two stations
- Accelerate and brake realistically. The strength of the acceleration and brake decrease when the speed rises, and it is mathematically impossible to go over a certain speed (will you find this maximum speed?).
- Multiple track profiles: straight, curved, flat, uphill, downhill, with connections with the opposite lanes... The ambiant sound adapts to the current track profile and your speed, and the acceleration and braking are influenced by the slope of the track: remember it before missing a station due to an excessive speed in a slope going down!
- Other trains are driving on the same line. They take speed limits into account, let passengers enter and leave, and beware not to collide with other trains, including yours.
- Railway signals are displayed, their color changes dynamically according to the position of the trains. Red lights should never be crossed.

## Extras
- Between two stations of the middle of the line, the lines goes above ground! Over a bridge, you will not see a nice city as the outdoor design is not someting I really worked on, but you will at least get a day-night cycle according to your system's date in the year and time in the day. You will also see the shadows of the bridge evolve over time.
- Near the beginning of the line, the line layout changed a few times. This results in the presence of a ghost station "République", near the actual station, that is not anymore inside the line but still visible. If you stop at République, you can go out and visit it - the quay is used to host technical locals.
- Your train is of perfect quality and will always work properly. But the other trains may fall into issues that will delay them in station for a random duration up to 10 minutes. If you are not too far away, you may experience the consequences (red signals and more passengers since the previous trains were full).

## Technical aspects
I do not share the executable file here. You can clone this repo and compile the project manually, but you will not be able to run the application due to the lack of data files and the non-relative file imports inside the project: this project has always been created by me and for me, and I have never considered sharing the executable. This repo's only goal is to present this project. 
The code is entirely written in C++, using Qt framework and the OpenGL library.
