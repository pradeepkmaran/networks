BEGIN{
    totRecv=0;
}

{
    type = $1;
    sz = $6;
    if(type == "r") {
        totRecv += sz;
    }
}

END{
    printf("Total size received : %d", totRecv);
}