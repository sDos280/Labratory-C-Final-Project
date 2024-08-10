
lab: add #55, r3
    .extern ppf
    .entry lab
    add #55, ppf
    add #55, fdf


data:
    .data 5156, 6, 151
    .extern fdf
    .entry data
    .string "ASDfas"