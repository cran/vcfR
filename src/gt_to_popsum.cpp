#include <Rcpp.h>

using namespace Rcpp;


std::vector < int > gtsplit(std::string line){
//Rcpp::IntegerVector gtsplit(std::string line){
//  Rcpp::IntegerVector intv;
  std::vector < int > intv;
//  Rcout << "Genotype: " << line << "\n";

  // Case of a single digit.
  if(line.size() == 1){
    intv.push_back(atoi(line.c_str()));
  }
  
  int start=0;
  for(int i=1; i<line.size(); i++){
    if( line[i] == '/' || line[i] == '|' ){
      std::string temp = line.substr(start, i);
//      Rcout << "  i: " << i << ", temp: " << temp << "\n";
      intv.push_back(atoi(temp.c_str()));
      start = i+1;
      i = i+2;
    }
  }

  // Handle last element.
  std::string temp = line.substr(start, line.size());
//  Rcout << "  temp: " << temp << "\n";
  intv.push_back(atoi(temp.c_str()));
  
  return intv;
}



// [[Rcpp::export]]
Rcpp::DataFrame gt_to_popsum(Rcpp::DataFrame var_info, Rcpp::CharacterMatrix gt) {
  // Calculate popgen summaries for the sample.
  // var_info should contain columns named 'CHROM', 'POS', 'mask' and possibly others.
  Rcpp::LogicalVector   mask = var_info["mask"];
  Rcpp::IntegerVector   nsample(mask.size());
  Rcpp::StringVector    allele_counts(mask.size());
  Rcpp::NumericVector   Hes(mask.size());
  Rcpp::NumericVector   Nes(mask.size());
  
  int i = 0;
  int j = 0;
  int k = 0;
  
  for(i=0; i < gt.nrow(); i++){ // Iterate over variants (rows)
    if(mask[i] == TRUE){
      std::vector<int> myalleles (1,0);
      for(j=0; j < gt.ncol(); j++){ // Iterate over samples (columns)
        if(gt(i, j) != NA_STRING){
          nsample[i]++;  // Increment sample count.

          // Count alleles per sample.
          std::vector < int > intv = gtsplit(as<std::string>(gt(i, j)));
          for(k=0; k<intv.size(); k++){
            while(myalleles.size() - 1 < intv[k]){
              // We have more alleles than exist in the vector myalleles.
              myalleles.push_back(0);
            }
            myalleles[intv[k]]++;
          }
        }
      }

      // Concatenate allele counts into a comma delimited string.
      int n;
      char buffer [50];
      n = sprintf (buffer, "%d", myalleles[0]);
      for(j=1; j < myalleles.size(); j++){
        n=sprintf (buffer, "%s,%d", buffer, myalleles[j]);
      }
      allele_counts[i] = buffer;

      // Sum all alleles.
      int nalleles = myalleles[0];
      for(j=1; j < myalleles.size(); j++){
        nalleles = nalleles + myalleles[j];
      }

      // Stats.
      double He = 1;
      He = He - pow(double(myalleles[0])/double(nalleles), myalleles.size());
      for(j=1; j < myalleles.size(); j++){
        He = He - pow(double(myalleles[j])/double(nalleles), myalleles.size());
      }
      Hes[i] = He;
      Nes[i] = 1/(1-He);
    } else { // Missing variant (row=NA)
      nsample[i] = NA_INTEGER;     
    }
  }

  return Rcpp::DataFrame::create(var_info, 
      _["n"]=nsample, 
      _["Allele_counts"]=allele_counts,
      _["He"]=Hes,
      _["Ne"]=Nes
  );
}
