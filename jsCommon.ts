export const postError = (e: any, m: string | null= null) => {
    if(!!m){
        error(m + "\n");
    } 
    error(JSON.stringify(e).replace(/\\u000a/gi,"    ") + "\n");
}