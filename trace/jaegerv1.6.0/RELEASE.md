# Jaeger Backend Release Process

1. Create a PR "Preparing release X.Y.Z" against master branch ([example](https://github.com/jaegertracing/jaeger/pull/543/files)) by updating CHANGELOG.md to include:
    * A new section with the header `<X.Y.Z> (YYYY-MM-DD)`
    * A curated list of notable changes and links to PRs. Do not simply dump git log, select the changes that affect the users.
    * The section can be split into sub-section if necessary, e.g. UI Changes, Backend Changes, Bug Fixes, etc.
2. After the PR is merged, create a release on Github:
    * Title "Release X.Y.Z" 
    * Tag `vX.Y.Z` (note the `v` prefix)
    * Copy the new CHANGELOG.md section into the release notes
3. The release tag will trigger a build of the docker images
4. Once the images are available on [Docker Hub](https://hub.docker.com/r/jaegertracing/), announce the release on the mailing list, gitter, and twitter.
