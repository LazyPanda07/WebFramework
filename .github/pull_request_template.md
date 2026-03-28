## Versions update
- [ ] WebFramework(WebFrameworkCApi.cpp)
- [ ] CMakeLists.txt
- [ ] distribution.yml(deb-package-version, deb-package-release-number)
- [ ] docker_distribution.yml(WEB_FRAMEWORK_DEPLOY_VERSION, WEB_FRAMEWORK_DEV_VERSION)
- [ ] deb_packages/web-framework
- [ ] deb_packages/web-framework-dev
- [ ] Docker/web_framework_deploy(WEB_FRAMEWORK_TAG, web-framework)
- [ ] Docker/web_framework_dev(web-framework-dev)


## API versions update
- [ ] C#(package version, assembly version, file version)
- [ ] Flutter(pubspec.yml, CHANGELOG.md)
- [ ] Python(pyproject.toml, CMakeLists.txt)


## Post automatic CI/CD
1. Run [Distribution workflow](https://github.com/LazyPanda07/WebFramework/actions/workflows/distribution.yml)
2. Download ```flutter-package``` from ```Distribution``` workflow and run 
```console
flutter pub publish
```
3. Run [Docker Distribution workflow](https://github.com/LazyPanda07/WebFramework/actions/workflows/docker_distribution.yml)
