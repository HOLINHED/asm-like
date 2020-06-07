MESSAGE=${1?Error: No commit message provided.}

git add .
git commit -s -m "$MESSAGE"
git push origin master