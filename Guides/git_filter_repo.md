**Installing git_filter_repo**   

Python and Pip are previously required.


```powershell
pip install --user git-filter-repo
```

**Using git_filter_repo**

```powershell
python -m git_filter_repo --analyze # Creates analysis files on .git\filter-repo\analysis
python -m git_filter_repo --path "file_to_remove.ext" --invert-paths 
python -m git_filter_repo --path "dir/to/remove/" --invert-paths # Trailing slash is needed for dirs.
python -m git_filter_repo --path "dir/to/remove/" --invert-paths --dry-run # Runs command in dry mode.
git add -A
git commit -m "Removing heavy files."
git push --force

```

**References**

- [How do you install git-filter-repo?](https://superuser.com/questions/1563034/how-do-you-install-git-filter-repo)   
- [newren/git-filter-repo](https://github.com/newren/git-filter-repo)