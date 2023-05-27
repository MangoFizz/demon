int halo_unknown_00478d40(int *p) {
    int a = p[3];
    int b = p[4];
    int c = p[0];

    if(b <= a) {
        return a - b;
    }
    else {
        return (a - b) * c;
    }
}
